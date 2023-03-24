import 'dart:ui';

import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'pushNotificationManager.dart';
void main() async {
  DartPluginRegistrant.ensureInitialized();
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      darkTheme: ThemeData.dark(),
      home: HomePage(),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  TextEditingController _dataController = TextEditingController();
  final databaseReference =
      FirebaseDatabase.instance.ref("test").child("relay");
  final Future<FirebaseApp> _future = Firebase.initializeApp();
  String data = "";
  String dataHumedad = "";
  PushNotificationManager manager = PushNotificationManager();
  FirebaseMessaging messaging = FirebaseMessaging.instance;



  void addData(String data) {
    //databaseReference.set({'valor': data});
    databaseReference.set(data);
  }

  String printFromDatabase() {
 /*   databaseReference.once().then((DatabaseEvent event) {
      print(event.snapshot.value);
    });*/

    DatabaseReference _relayreference =
        FirebaseDatabase.instance.ref("test").child("temperatura");
    _relayreference.onValue.listen((DatabaseEvent event) {
      final data = event.snapshot.value.toString();
      setState(() {
        this.data = data;
      });
      print(data);
    });
    return data;
  }
  String getHumidity() {

    DatabaseReference _relayreference =
    FirebaseDatabase.instance.ref("test").child("humedad");
    _relayreference.onValue.listen((DatabaseEvent event) {
      final data = event.snapshot.value.toString();
      setState(() {
        this.dataHumedad = data;
      });
      print(data);
    });
    return dataHumedad;
  }

  @override
  void initState() {
    FirebaseMessaging.onMessage.listen((RemoteMessage message) {
      RemoteNotification? notification = message.notification;
      AndroidNotification? android = message.notification?.android;
      print(notification?.body.toString());
      print(notification?.title.toString());
    });

    FirebaseMessaging.onMessageOpenedApp.listen((RemoteMessage message) {
      print('A new onMessageOpenedApp event was published!');
      RemoteNotification? notification = message.notification;
      AndroidNotification? android = message.notification?.android;
      print(notification?.body.toString());
      print(notification?.title.toString());
    });
    // TODO: implement initState
    super.initState();
    printFromDatabase();
    getHumidity();
    manager.init();

  }

  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    PushNotificationManager manager = PushNotificationManager();
    FirebaseMessaging messaging = FirebaseMessaging.instance;

    var color_on = Colors.yellowAccent;
    var color_off = Colors.blueAccent;
    var flag = true;
    return Scaffold(
      appBar: AppBar(
        title: Text("Firebase Listener"),
      ),
      body: FutureBuilder(
        future: _future,
        builder: (context, snapshot) {
          if (snapshot.hasError) {
            return Text(snapshot.error.toString());
          } else {
            return Column(
              children: [
                const SizedBox(
                  height: 70,
                ),
                Text(
                  "Temperatura $data"+" °C",
                  style: const TextStyle(fontSize: 30, color: Colors.green),
                ),
                const SizedBox(
                  height: 70,
                ),
                Text(
                  "Humedad Relativa $dataHumedad"+" %",
                  style: const TextStyle(fontSize: 30, color: Colors.green),
                ),
                const SizedBox(
                  height: 50,
                ),
                Padding(
                  padding: const EdgeInsets.all(10),
                  /* child: TextField(
                    controller: _dataController,
                    decoration: const InputDecoration(hintText: "Insert Data"),
                  ),*/
                ),
                const SizedBox(
                  height: 40,
                ),
                Center(
                  child: ElevatedButton(
                    onPressed: () {
                      addData("1");
                    },
                    style: ElevatedButton.styleFrom(
                      primary: color_on,
                      onPrimary: Colors.black,
                      shape: const RoundedRectangleBorder(
                        borderRadius: BorderRadius.all(Radius.circular(10)),
                      ),
                    ),
                    child: const Text("ON"),

                  ),

                ),
                const SizedBox(
                  height: 40,
                ),
                Column(
                  children: [
                    Center(
                      child: ElevatedButton(
                        onPressed: () {
                          addData("0");
                        },
                        style: ElevatedButton.styleFrom(
                          primary: color_off,
                          onPrimary: Colors.black,
                          shape: const RoundedRectangleBorder(
                            borderRadius: BorderRadius.all(Radius.circular(10)),
                          ),
                        ),
                        child: const Text("OFF"),

                      ),
                    )
                  ],
                ),
                const SizedBox(
                  height: 40,
                ),
              ],
            );
          }
        },
      ),
    );
  }
}
