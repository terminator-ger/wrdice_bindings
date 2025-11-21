
import 'package:flutter/material.dart';
import 'dart:async';
import 'package:wrdice/wrdice.dart' as wrdice;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  late Future<wrdice.DartSimStats> asyncResult;
  @override
  void initState() {
    super.initState();
    final wrdice.DartArmy da = wrdice.DartArmy(<int>[3,5,3,0,0],
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               <int>[0,0,-1,-1,-1],
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               );
      final wrdice.DartArmy db = wrdice.DartArmy(<int>[3,5,3,0,0],
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               <int>[0,0,-1,-1,-1],
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               List<int>.filled(5,0),
                                               );
 
    
    final bool fa = true;
    final bool bc = true;
    asyncResult = wrdice.runBattleAsync(da, db, fa,  bc);
  }
  
  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(fontSize: 25);
    const spacerSmall = SizedBox(height: 10);
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Native Packages'),
        ),
        body: SingleChildScrollView(
          child: Container(
            padding: const .all(10),
            child: Column(
              children: [
                spacerSmall,
                FutureBuilder<wrdice.DartSimStats>(
                  future: asyncResult,
                  builder: (BuildContext context, AsyncSnapshot<wrdice.DartSimStats> value) {
                    final displayValue =
                        (value.hasData) ? value.data?.br.winA : 'loading';
                    return Text(
                      'await sumAsync(3, 4) = $displayValue',
                      style: textStyle,
                      textAlign: .center,
                    );
                  },
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
