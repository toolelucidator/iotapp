import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_messaging/firebase_messaging.dart';

class PushNotificationManager {
  PushNotificationManager._();

  factory PushNotificationManager() => PushNotificationManager._();
  final FirebaseMessaging _firebaseMessaging = FirebaseMessaging.instance;
  bool _initialized = false;

  Future<void> init() async {
    await Firebase.initializeApp();
    if (!_initialized) {
      // For iOS request permission first.
      _firebaseMessaging.requestPermission();
      // For testing purposes print the Firebase Messaging token
      String? token = await _firebaseMessaging.getToken();
      print("FirebaseMessaging token: $token");
      _initialized = true;
    }
  }
}