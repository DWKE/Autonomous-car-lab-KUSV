
(cl:in-package :asdf)

(defsystem "kusv_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :sensor_msgs-msg
)
  :components ((:file "_package")
    (:file "ControlCmd" :depends-on ("_package_ControlCmd"))
    (:file "_package_ControlCmd" :depends-on ("_package"))
    (:file "DetectedObject" :depends-on ("_package_DetectedObject"))
    (:file "_package_DetectedObject" :depends-on ("_package"))
    (:file "PolyfitLaneData" :depends-on ("_package_PolyfitLaneData"))
    (:file "_package_PolyfitLaneData" :depends-on ("_package"))
    (:file "PolyfitLaneDataArray" :depends-on ("_package_PolyfitLaneDataArray"))
    (:file "_package_PolyfitLaneDataArray" :depends-on ("_package"))
    (:file "Waypoint" :depends-on ("_package_Waypoint"))
    (:file "_package_Waypoint" :depends-on ("_package"))
    (:file "WaypointArray" :depends-on ("_package_WaypointArray"))
    (:file "_package_WaypointArray" :depends-on ("_package"))
  ))