;; Auto-generated. Do not edit!


(when (boundp 'kusv_msgs::ControlCmd)
  (if (not (find-package "KUSV_MSGS"))
    (make-package "KUSV_MSGS"))
  (shadow 'ControlCmd (find-package "KUSV_MSGS")))
(unless (find-package "KUSV_MSGS::CONTROLCMD")
  (make-package "KUSV_MSGS::CONTROLCMD"))

(in-package "ROS")
;;//! \htmlinclude ControlCmd.msg.html


(defclass kusv_msgs::ControlCmd
  :super ros::object
  :slots (_linear_velocity _steering_angle ))

(defmethod kusv_msgs::ControlCmd
  (:init
   (&key
    ((:linear_velocity __linear_velocity) 0.0)
    ((:steering_angle __steering_angle) 0.0)
    )
   (send-super :init)
   (setq _linear_velocity (float __linear_velocity))
   (setq _steering_angle (float __steering_angle))
   self)
  (:linear_velocity
   (&optional __linear_velocity)
   (if __linear_velocity (setq _linear_velocity __linear_velocity)) _linear_velocity)
  (:steering_angle
   (&optional __steering_angle)
   (if __steering_angle (setq _steering_angle __steering_angle)) _steering_angle)
  (:serialization-length
   ()
   (+
    ;; float64 _linear_velocity
    8
    ;; float64 _steering_angle
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _linear_velocity
       (sys::poke _linear_velocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steering_angle
       (sys::poke _steering_angle (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _linear_velocity
     (setq _linear_velocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steering_angle
     (setq _steering_angle (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get kusv_msgs::ControlCmd :md5sum-) "6f32152926ab6255084d804f9e199d25")
(setf (get kusv_msgs::ControlCmd :datatype-) "kusv_msgs/ControlCmd")
(setf (get kusv_msgs::ControlCmd :definition-)
      "float64 linear_velocity
float64 steering_angle

")



(provide :kusv_msgs/ControlCmd "6f32152926ab6255084d804f9e199d25")


