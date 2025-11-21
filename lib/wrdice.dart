import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';
import 'package:ffi/ffi.dart';
import 'wrdice_bindings_generated.dart';

//
// ───────────────────────────────────────────────
//   Function Binding
// ───────────────────────────────────────────────
//

// C: void run_simulation(const Army*, const Army*, SimStats*, bool, bool)
typedef _RunSimulationNative = Void Function(
  Pointer<Army> armyA,
  Pointer<Army> armyB,
  Pointer<SimStats> stats,
  Uint8 withForceAdvantage,  // bool → uint8
  Uint8 withBatchCap,        // bool → uint8
);

typedef RunSimulation = void Function(
  Pointer<Army> armyA,
  Pointer<Army> armyB,
  Pointer<SimStats> stats,
  bool withForceAdvantage,
  bool withBatchCap,
);


class BattleSimLib {
  late final DynamicLibrary _lib;
  late final RunSimulation runSimulation;
  final String _libName = 'wrdice';
  late final WrdiceBindings _bindings;

  BattleSimLib() {
    _lib = () {
      if (Platform.isMacOS || Platform.isIOS) {
        return DynamicLibrary.open('$_libName.framework/$_libName');
      }
      if (Platform.isAndroid || Platform.isLinux) {
        return DynamicLibrary.open('lib$_libName.so');
      }
      if (Platform.isWindows) {
        return DynamicLibrary.open('$_libName.dll');
      }
      throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
    }();
    _bindings = WrdiceBindings(_lib);
    runSimulation = _bindings.run_simulation;
    //lookup<_RunSimulationNative>('run_simulation');
    //runSimulation = _lib
    //    .lookupFunction<_RunSimulationNative, RunSimulation>('run_simulation');
  }
}
//
// ───────────────────────────────────────────────
//   Helper Extensions (optional)
// ───────────────────────────────────────────────
//

extension ArrayUint32Ext on Array<Uint32> {
  List<int> toList(int n) =>
      List<int>.generate(n, (i) => this[i]);
}

extension ArrayFloatExt on Array<Float> {
  List<double> toList(int n) =>
      List<double>.generate(n, (i) => this[i]);
}

extension SurvivedExt on Survived {
  List<int> getCounts() => count.toList(256);
  List<double> getOdds() => odds.toList(256);
}

/// Fills a Stance struct from lists of offensive and defensive values
void _fillStance(Stance stance, List<int> offensive, List<int> defensive) {
  assert(offensive.length >= 5, 'Offensive list must have at least 5 elements');
  assert(defensive.length >= 5, 'Defensive list must have at least 5 elements');
  
  for (int i = 0; i < 5; i++) {
    stance.stance_off[i] = offensive[i];
    stance.stance_def[i] = defensive[i];
  }
}


class DartArmy{
  late final List<int> unitsLnd;
  late final List<int> unitsAir;
  late final List<int> unitsSea;
  late final List<int> stanceLndOff;
  late final List<int> stanceLndDef;
  late final List<int> stanceAirOff;
  late final List<int> stanceAirDef;
  late final List<int> stanceSeaOff;
  late final List<int> stanceSeaDef;
  DartArmy(this.unitsLnd, this.unitsSea, this.unitsAir, this.stanceLndOff, this.stanceLndDef, this.stanceAirOff, this.stanceAirDef, this.stanceSeaOff, this.stanceSeaDef);
}

/// Fills an Army struct from Dart lists
/// 
/// Parameters:
///   - armyPtr: Pointer to the Army struct to fill
///   - unitsLnd: List of 5 land unit counts
///   - unitsAir: List of 5 air unit counts
///   - unitsSea: List of 5 sea unit counts
///   - stanceLndOff: List of 5 land offensive stances
///   - stanceLndDef: List of 5 land defensive stances
///   - stanceAirOff: List of 5 air offensive stances
///   - stanceAirDef: List of 5 air defensive stances
///   - stanceSeaOff: List of 5 sea offensive stances
///   - stanceSeaDef: List of 5 sea defensive stances

void fillArmy(Pointer<Army> armyPtr, DartArmy army){
  // Fill unit counts
  final army_ = armyPtr.ref;
  for (int i = 0; i < 5; i++) {
    army_.n_units_lnd[i] = army.unitsLnd[i];
    army_.n_units_air[i] = army.unitsAir[i];
    army_.n_units_sea[i] = army.unitsSea[i];
  }
  _fillStance(army_.stance_lnd, army.stanceLndOff, army.stanceLndDef);
  _fillStance(army_.stance_lnd, army.stanceLndOff, army.stanceLndDef);
 
}

void fillArmyFromLists(
  Pointer<Army>armyPtr, {
  required List<int> unitsLnd,
  required List<int> unitsAir,
  required List<int> unitsSea,
  required List<int> stanceLndOff,
  required List<int> stanceLndDef,
  required List<int> stanceAirOff,
  required List<int> stanceAirDef,
  required List<int> stanceSeaOff,
  required List<int> stanceSeaDef,
}) {
  final army = armyPtr.ref;
  
  // Fill unit counts
  for (int i = 0; i < 5; i++) {
    army.n_units_lnd[i] = unitsLnd[i];
    army.n_units_air[i] = unitsAir[i];
    army.n_units_sea[i] = unitsSea[i];
  }
  
  // Fill stances
  _fillStance(army.stance_lnd, stanceLndOff, stanceLndDef);
  _fillStance(army.stance_air, stanceAirOff, stanceAirDef);
  _fillStance(army.stance_sea, stanceSeaOff, stanceSeaDef);
}

// ─────────────────────────────
//   Dart Models (Plain Objects)
// ─────────────────────────────


class DartBattleRequest {
   final int id;
   final Pointer<Army> a;
   final Pointer<Army> b;
   final bool withForceAdvantage;
   final bool withBatchCap;
   const DartBattleRequest(this.id, this.a, this.b, this.withForceAdvantage, this.withBatchCap);
}

class DartBattleResponse {
  final int id;
  final DartSimStats stats;

  DartBattleResponse({required this.id, required this.stats});
}

class DartBattleResult {
  final double winA;
  final double winB;
  final double draw;
  final double death;

  DartBattleResult({
    required this.winA,
    required this.winB,
    required this.draw,
    required this.death,
  });

  @override
  String toString() =>
      "BattleResult(winA: $winA, winB: $winB, draw: $draw, death: $death)";
}

class DartSurvived {
  final int size;
  final List<int> count; // length 256
  final List<double> odds; // length 256

  DartSurvived({
    required this.size,
    required this.count,
    required this.odds,
  });
}

class DartStats {
  final List<DartSurvived> land; // length 5
  final List<DartSurvived> air;  // length 5
  final List<DartSurvived> sea;  // length 5

  DartStats({
    required this.land,
    required this.air,
    required this.sea,
  });
}

class DartSimStats {
  final int id;
  final DartStats armyA;
  final DartStats armyB;
  final DartBattleResult br;

  DartSimStats({
    required this.id,
    required this.armyA,
    required this.armyB,
    required this.br,
  });
}



// ─────────────────────────────
//   FFI → Dart Conversion
// ─────────────────────────────

DartBattleResult _toBattleResult(BattleResult br) {
  return DartBattleResult(
    winA: br.win_a,
    winB: br.win_b,
    draw: br.draw,
    death: br.death,
  );
}

DartSurvived _toSurvived(Survived s) {
  return DartSurvived(
    size: s.size,
    count: s.count.toList(256),
    odds: s.odds.toList(256),
  );
}

List<DartSurvived> _extractSurvivedArray(Array<Survived> arr, int n) {
  return List.generate(n, (i) => _toSurvived(arr[i]));
}

DartStats _toStats(Stats st) {
  return DartStats(
    land: _extractSurvivedArray(st.stats_lnd, 5),
    air:  _extractSurvivedArray(st.stats_air, 5),
    sea:  _extractSurvivedArray(st.stats_sea, 5),
  );
}

DartSimStats toDartSimStats(Pointer<SimStats> ptr) {
  final ref = ptr.ref;

  return DartSimStats(
    id: 0,
    armyA: _toStats(ref.army_a),
    armyB: _toStats(ref.army_b),
    br: _toBattleResult(ref.br),
  );
}


DartBattleResponse runBattleSim(
  BattleSimLib lib,
  Pointer<Army> armyA,
  Pointer<Army> armyB,
  bool withForceAdvantage,
  bool withBatchCap,
  int id
){
  final statsPtr = calloc<SimStats>();

  lib.runSimulation(
    armyA,
    armyB,
    statsPtr,
    withForceAdvantage, // with_force_advantage = true
    withBatchCap // with_batch_cap = true
  );

  final stats = toDartSimStats(statsPtr);

  calloc.free(statsPtr);
  DartBattleResponse resp = DartBattleResponse(id: id, stats: stats);

  return resp;
}


/// Counter to identify [_SumRequest]s and [_SumResponse]s.
int _nextBattleRequestId = 0;

/// Mapping from [_SumRequest] `id`s to the completers corresponding to the correct future of the pending request.
final Map<int, Completer<DartSimStats>> _battleRequest = <int, Completer<DartSimStats>>{};

/// The SendPort belonging to the helper isolate.
Future<SendPort> _helperIsolateSendPort = () async {
  // The helper isolate is going to send us back a SendPort, which we want to
  // wait for.
  final Completer<SendPort> completer = Completer<SendPort>();

  // Receive port on the main isolate to receive messages from the helper.
  // We receive two types of messages:
  // 1. A port to send messages on.
  // 2. Responses to requests we sent.
  final ReceivePort receivePort = ReceivePort()
    ..listen((dynamic data) {
      if (data is SendPort) {
        // The helper isolate sent us the port on which we can sent it requests.
        completer.complete(data);
        return;
      }
      if (data is DartBattleResponse) {
        // The helper isolate sent us a response to a request we sent.
        if (_battleRequest.containsKey(data.id)){
          final Completer<DartSimStats>? completer = _battleRequest[data.id];
          _battleRequest.remove(data.id);
          completer?.complete(data.stats);
          return;
        }
      }
      throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
    });

  // Start the helper isolate.
  await Isolate.spawn((SendPort sendPort) async {
    final ReceivePort helperReceivePort = ReceivePort()
      ..listen((dynamic data) {
        // On the helper isolate listen to requests and respond to them.
        if (data is DartBattleRequest) {
          BattleSimLib lib = BattleSimLib();
          final DartBattleResponse result = runBattleSim(lib, data.a, data.b, data.withForceAdvantage, data.withBatchCap, data.id);
          sendPort.send(result);
          return;
        }
        throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
      });

    // Send the port to the main isolate on which we can receive requests.
    sendPort.send(helperReceivePort.sendPort);
  }, receivePort.sendPort);

  // Wait until the helper isolate has sent us back the SendPort on which we
  // can start sending requests.
  return completer.future;
}();

Future<DartSimStats> runBattleAsync(DartArmy da, DartArmy db, bool fa, bool bc) async {
  final SendPort helperIsolateSendPort = await _helperIsolateSendPort;
  final int requestId = _nextBattleRequestId++;
  final Pointer<Army> a = calloc<Army>();
  final Pointer<Army> b = calloc<Army>();
  fillArmy(a, da);
  fillArmy(b, db);
  final DartBattleRequest request = DartBattleRequest(requestId, a, b, fa, bc);
  final Completer<DartSimStats> completer = Completer<DartSimStats>();
  _battleRequest[requestId] = completer;
  helperIsolateSendPort.send(request);
  return completer.future;
}