;; Auto-generated. Do not edit!


(when (boundp 'kusv_msgs::DetectedObject)
  (if (not (find-package "KUSV_MSGS"))
    (make-package "KUSV_MSGS"))
  (shadow 'DetectedObject (find-package "KUSV_MSGS")))
(unless (find-package "KUSV_MSGS::DETECTEDOBJECT")
  (make-package "KUSV_MSGS::DETECTEDOBJECT"))

(in-package "ROS")
;;//! \htmlinclude DetectedObject.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass kusv_msgs::DetectedObject
  :super ros::object
  :slots (_id _label _pose _convex_hull ))

(defmethod kusv_msgs::DetectedObject
  (:init
   (&key
    ((:id __id) 0)
    ((:label __label) "")
    ((:pose __pose) (instance geometry_msgs::Pose :init))
    ((:convex_hull __convex_hull) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point :init) r)) r))
    )
   (send-super :init)
   (setq _id (round __id))
   (setq _label (string __label))
   (setq _pose __pose)
   (setq _convex_hull __convex_hull)
   self)
  (:id
   (&optional __id)
   (if __id (setq _id __id)) _id)
  (:label
   (&optional __label)
   (if __label (setq _label __label)) _label)
  (:pose
   (&rest __pose)
   (if (keywordp (car __pose))
       (send* _pose __pose)
     (progn
       (if __pose (setq _pose (car __pose)))
       _pose)))
  (:convex_hull
   (&rest __convex_hull)
   (if (keywordp (car __convex_hull))
       (send* _convex_hull __convex_hull)
     (progn
       (if __convex_hull (setq _convex_hull (car __convex_hull)))
       _convex_hull)))
  (:serialization-length
   ()
   (+
    ;; uint32 _id
    4
    ;; string _label
    4 (length _label)
    ;; geometry_msgs/Pose _pose
    (send _pose :serialization-length)
    ;; geometry_msgs/Point[] _convex_hull
    (apply #'+ (send-all _convex_hull :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _id
       (write-long _id s)
     ;; string _label
       (write-long (length _label) s) (princ _label s)
     ;; geometry_msgs/Pose _pose
       (send _pose :serialize s)
     ;; geometry_msgs/Point[] _convex_hull
     (write-long (length _convex_hull) s)
     (dolist (elem _convex_hull)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _id
     (setq _id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _label
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _label (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; geometry_msgs/Pose _pose
     (send _pose :deserialize buf ptr-) (incf ptr- (send _pose :serialization-length))
   ;; geometry_msgs/Point[] _convex_hull
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _convex_hull (let (r) (dotimes (i n) (push (instance geometry_msgs::Point :init) r)) r))
     (dolist (elem- _convex_hull)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get kusv_msgs::DetectedObject :md5sum-) "cb70f119525e9d60b681d941cb0e2d32")
(setf (get kusv_msgs::DetectedObject :datatype-) "kusv_msgs/DetectedObject")
(setf (get kusv_msgs::DetectedObject :definition-)
      "uint32 id
string label

geometry_msgs/Pose pose
geometry_msgs/Point[] convex_hull

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



(provide :kusv_msgs/DetectedObject "cb70f119525e9d60b681d941cb0e2d32")


