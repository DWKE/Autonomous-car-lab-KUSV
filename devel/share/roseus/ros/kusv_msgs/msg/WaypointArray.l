;; Auto-generated. Do not edit!


(when (boundp 'kusv_msgs::WaypointArray)
  (if (not (find-package "KUSV_MSGS"))
    (make-package "KUSV_MSGS"))
  (shadow 'WaypointArray (find-package "KUSV_MSGS")))
(unless (find-package "KUSV_MSGS::WAYPOINTARRAY")
  (make-package "KUSV_MSGS::WAYPOINTARRAY"))

(in-package "ROS")
;;//! \htmlinclude WaypointArray.msg.html


(defclass kusv_msgs::WaypointArray
  :super ros::object
  :slots (_waypoints ))

(defmethod kusv_msgs::WaypointArray
  (:init
   (&key
    ((:waypoints __waypoints) (let (r) (dotimes (i 0) (push (instance kusv_msgs::Waypoint :init) r)) r))
    )
   (send-super :init)
   (setq _waypoints __waypoints)
   self)
  (:waypoints
   (&rest __waypoints)
   (if (keywordp (car __waypoints))
       (send* _waypoints __waypoints)
     (progn
       (if __waypoints (setq _waypoints (car __waypoints)))
       _waypoints)))
  (:serialization-length
   ()
   (+
    ;; kusv_msgs/Waypoint[] _waypoints
    (apply #'+ (send-all _waypoints :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; kusv_msgs/Waypoint[] _waypoints
     (write-long (length _waypoints) s)
     (dolist (elem _waypoints)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; kusv_msgs/Waypoint[] _waypoints
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _waypoints (let (r) (dotimes (i n) (push (instance kusv_msgs::Waypoint :init) r)) r))
     (dolist (elem- _waypoints)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get kusv_msgs::WaypointArray :md5sum-) "bd8cd3bad318acea3b5f67f7eb14b5d3")
(setf (get kusv_msgs::WaypointArray :datatype-) "kusv_msgs/WaypointArray")
(setf (get kusv_msgs::WaypointArray :definition-)
      "Waypoint[] waypoints

================================================================================
MSG: kusv_msgs/Waypoint
geometry_msgs/PoseStamped pose

================================================================================
MSG: geometry_msgs/PoseStamped
# A Pose with reference coordinate frame and timestamp
Header header
Pose pose

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

")



(provide :kusv_msgs/WaypointArray "bd8cd3bad318acea3b5f67f7eb14b5d3")


