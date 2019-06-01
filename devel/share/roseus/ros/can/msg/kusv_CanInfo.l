;; Auto-generated. Do not edit!


(when (boundp 'can::kusv_CanInfo)
  (if (not (find-package "CAN"))
    (make-package "CAN"))
  (shadow 'kusv_CanInfo (find-package "CAN")))
(unless (find-package "CAN::KUSV_CANINFO")
  (make-package "CAN::KUSV_CANINFO"))

(in-package "ROS")
;;//! \htmlinclude kusv_CanInfo.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass can::kusv_CanInfo
  :super ros::object
  :slots (_header _speedfr _speedfl _speedrr _speedrl _speed_avr_r _lat_acc_speed _parking_brake_on _air_cond_on _steering_wheel_angle _steering_wheel_angular _steering_wheel_tq _pos_acc_pedal _brake_active_state _brake_cylinder_press _engine_speed _gear_trans_state _gear_seldisp _pos_throttle _cluster_odometer _lon_acc_speed _vehicle_speed_engine _yaw_rate ))

(defmethod can::kusv_CanInfo
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:speedfr __speedfr) 0.0)
    ((:speedfl __speedfl) 0.0)
    ((:speedrr __speedrr) 0.0)
    ((:speedrl __speedrl) 0.0)
    ((:speed_avr_r __speed_avr_r) 0.0)
    ((:lat_acc_speed __lat_acc_speed) 0.0)
    ((:parking_brake_on __parking_brake_on) 0)
    ((:air_cond_on __air_cond_on) 0)
    ((:steering_wheel_angle __steering_wheel_angle) 0.0)
    ((:steering_wheel_angular __steering_wheel_angular) 0)
    ((:steering_wheel_tq __steering_wheel_tq) 0.0)
    ((:pos_acc_pedal __pos_acc_pedal) 0.0)
    ((:brake_active_state __brake_active_state) 0)
    ((:brake_cylinder_press __brake_cylinder_press) 0.0)
    ((:engine_speed __engine_speed) 0.0)
    ((:gear_trans_state __gear_trans_state) 0)
    ((:gear_seldisp __gear_seldisp) 0)
    ((:pos_throttle __pos_throttle) 0.0)
    ((:cluster_odometer __cluster_odometer) 0.0)
    ((:lon_acc_speed __lon_acc_speed) 0.0)
    ((:vehicle_speed_engine __vehicle_speed_engine) 0)
    ((:yaw_rate __yaw_rate) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _speedfr (float __speedfr))
   (setq _speedfl (float __speedfl))
   (setq _speedrr (float __speedrr))
   (setq _speedrl (float __speedrl))
   (setq _speed_avr_r (float __speed_avr_r))
   (setq _lat_acc_speed (float __lat_acc_speed))
   (setq _parking_brake_on (round __parking_brake_on))
   (setq _air_cond_on (round __air_cond_on))
   (setq _steering_wheel_angle (float __steering_wheel_angle))
   (setq _steering_wheel_angular (round __steering_wheel_angular))
   (setq _steering_wheel_tq (float __steering_wheel_tq))
   (setq _pos_acc_pedal (float __pos_acc_pedal))
   (setq _brake_active_state (round __brake_active_state))
   (setq _brake_cylinder_press (float __brake_cylinder_press))
   (setq _engine_speed (float __engine_speed))
   (setq _gear_trans_state (round __gear_trans_state))
   (setq _gear_seldisp (round __gear_seldisp))
   (setq _pos_throttle (float __pos_throttle))
   (setq _cluster_odometer (float __cluster_odometer))
   (setq _lon_acc_speed (float __lon_acc_speed))
   (setq _vehicle_speed_engine (round __vehicle_speed_engine))
   (setq _yaw_rate (float __yaw_rate))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:speedfr
   (&optional __speedfr)
   (if __speedfr (setq _speedfr __speedfr)) _speedfr)
  (:speedfl
   (&optional __speedfl)
   (if __speedfl (setq _speedfl __speedfl)) _speedfl)
  (:speedrr
   (&optional __speedrr)
   (if __speedrr (setq _speedrr __speedrr)) _speedrr)
  (:speedrl
   (&optional __speedrl)
   (if __speedrl (setq _speedrl __speedrl)) _speedrl)
  (:speed_avr_r
   (&optional __speed_avr_r)
   (if __speed_avr_r (setq _speed_avr_r __speed_avr_r)) _speed_avr_r)
  (:lat_acc_speed
   (&optional __lat_acc_speed)
   (if __lat_acc_speed (setq _lat_acc_speed __lat_acc_speed)) _lat_acc_speed)
  (:parking_brake_on
   (&optional __parking_brake_on)
   (if __parking_brake_on (setq _parking_brake_on __parking_brake_on)) _parking_brake_on)
  (:air_cond_on
   (&optional __air_cond_on)
   (if __air_cond_on (setq _air_cond_on __air_cond_on)) _air_cond_on)
  (:steering_wheel_angle
   (&optional __steering_wheel_angle)
   (if __steering_wheel_angle (setq _steering_wheel_angle __steering_wheel_angle)) _steering_wheel_angle)
  (:steering_wheel_angular
   (&optional __steering_wheel_angular)
   (if __steering_wheel_angular (setq _steering_wheel_angular __steering_wheel_angular)) _steering_wheel_angular)
  (:steering_wheel_tq
   (&optional __steering_wheel_tq)
   (if __steering_wheel_tq (setq _steering_wheel_tq __steering_wheel_tq)) _steering_wheel_tq)
  (:pos_acc_pedal
   (&optional __pos_acc_pedal)
   (if __pos_acc_pedal (setq _pos_acc_pedal __pos_acc_pedal)) _pos_acc_pedal)
  (:brake_active_state
   (&optional __brake_active_state)
   (if __brake_active_state (setq _brake_active_state __brake_active_state)) _brake_active_state)
  (:brake_cylinder_press
   (&optional __brake_cylinder_press)
   (if __brake_cylinder_press (setq _brake_cylinder_press __brake_cylinder_press)) _brake_cylinder_press)
  (:engine_speed
   (&optional __engine_speed)
   (if __engine_speed (setq _engine_speed __engine_speed)) _engine_speed)
  (:gear_trans_state
   (&optional __gear_trans_state)
   (if __gear_trans_state (setq _gear_trans_state __gear_trans_state)) _gear_trans_state)
  (:gear_seldisp
   (&optional __gear_seldisp)
   (if __gear_seldisp (setq _gear_seldisp __gear_seldisp)) _gear_seldisp)
  (:pos_throttle
   (&optional __pos_throttle)
   (if __pos_throttle (setq _pos_throttle __pos_throttle)) _pos_throttle)
  (:cluster_odometer
   (&optional __cluster_odometer)
   (if __cluster_odometer (setq _cluster_odometer __cluster_odometer)) _cluster_odometer)
  (:lon_acc_speed
   (&optional __lon_acc_speed)
   (if __lon_acc_speed (setq _lon_acc_speed __lon_acc_speed)) _lon_acc_speed)
  (:vehicle_speed_engine
   (&optional __vehicle_speed_engine)
   (if __vehicle_speed_engine (setq _vehicle_speed_engine __vehicle_speed_engine)) _vehicle_speed_engine)
  (:yaw_rate
   (&optional __yaw_rate)
   (if __yaw_rate (setq _yaw_rate __yaw_rate)) _yaw_rate)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _speedfr
    8
    ;; float64 _speedfl
    8
    ;; float64 _speedrr
    8
    ;; float64 _speedrl
    8
    ;; float64 _speed_avr_r
    8
    ;; float64 _lat_acc_speed
    8
    ;; uint8 _parking_brake_on
    1
    ;; uint8 _air_cond_on
    1
    ;; float64 _steering_wheel_angle
    8
    ;; uint16 _steering_wheel_angular
    2
    ;; float64 _steering_wheel_tq
    8
    ;; float64 _pos_acc_pedal
    8
    ;; uint8 _brake_active_state
    1
    ;; float64 _brake_cylinder_press
    8
    ;; float64 _engine_speed
    8
    ;; uint8 _gear_trans_state
    1
    ;; uint8 _gear_seldisp
    1
    ;; float64 _pos_throttle
    8
    ;; float64 _cluster_odometer
    8
    ;; float64 _lon_acc_speed
    8
    ;; uint8 _vehicle_speed_engine
    1
    ;; float64 _yaw_rate
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64 _speedfr
       (sys::poke _speedfr (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _speedfl
       (sys::poke _speedfl (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _speedrr
       (sys::poke _speedrr (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _speedrl
       (sys::poke _speedrl (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _speed_avr_r
       (sys::poke _speed_avr_r (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _lat_acc_speed
       (sys::poke _lat_acc_speed (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; uint8 _parking_brake_on
       (write-byte _parking_brake_on s)
     ;; uint8 _air_cond_on
       (write-byte _air_cond_on s)
     ;; float64 _steering_wheel_angle
       (sys::poke _steering_wheel_angle (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; uint16 _steering_wheel_angular
       (write-word _steering_wheel_angular s)
     ;; float64 _steering_wheel_tq
       (sys::poke _steering_wheel_tq (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _pos_acc_pedal
       (sys::poke _pos_acc_pedal (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; uint8 _brake_active_state
       (write-byte _brake_active_state s)
     ;; float64 _brake_cylinder_press
       (sys::poke _brake_cylinder_press (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _engine_speed
       (sys::poke _engine_speed (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; uint8 _gear_trans_state
       (write-byte _gear_trans_state s)
     ;; uint8 _gear_seldisp
       (write-byte _gear_seldisp s)
     ;; float64 _pos_throttle
       (sys::poke _pos_throttle (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _cluster_odometer
       (sys::poke _cluster_odometer (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _lon_acc_speed
       (sys::poke _lon_acc_speed (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; uint8 _vehicle_speed_engine
       (write-byte _vehicle_speed_engine s)
     ;; float64 _yaw_rate
       (sys::poke _yaw_rate (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _speedfr
     (setq _speedfr (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _speedfl
     (setq _speedfl (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _speedrr
     (setq _speedrr (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _speedrl
     (setq _speedrl (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _speed_avr_r
     (setq _speed_avr_r (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _lat_acc_speed
     (setq _lat_acc_speed (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; uint8 _parking_brake_on
     (setq _parking_brake_on (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _air_cond_on
     (setq _air_cond_on (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _steering_wheel_angle
     (setq _steering_wheel_angle (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; uint16 _steering_wheel_angular
     (setq _steering_wheel_angular (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; float64 _steering_wheel_tq
     (setq _steering_wheel_tq (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _pos_acc_pedal
     (setq _pos_acc_pedal (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; uint8 _brake_active_state
     (setq _brake_active_state (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _brake_cylinder_press
     (setq _brake_cylinder_press (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _engine_speed
     (setq _engine_speed (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; uint8 _gear_trans_state
     (setq _gear_trans_state (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _gear_seldisp
     (setq _gear_seldisp (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _pos_throttle
     (setq _pos_throttle (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _cluster_odometer
     (setq _cluster_odometer (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _lon_acc_speed
     (setq _lon_acc_speed (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; uint8 _vehicle_speed_engine
     (setq _vehicle_speed_engine (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _yaw_rate
     (setq _yaw_rate (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get can::kusv_CanInfo :md5sum-) "0d05be062e80361fb3d556c45a03d561")
(setf (get can::kusv_CanInfo :datatype-) "can/kusv_CanInfo")
(setf (get can::kusv_CanInfo :definition-)
      "Header header
float64 speedfr
float64 speedfl
float64 speedrr
float64 speedrl
float64 speed_avr_r
float64 lat_acc_speed
uint8 parking_brake_on
uint8 air_cond_on
float64 steering_wheel_angle
uint16 steering_wheel_angular
float64 steering_wheel_tq
float64 pos_acc_pedal
uint8 brake_active_state
float64 brake_cylinder_press
float64 engine_speed
uint8 gear_trans_state
uint8 gear_seldisp
float64 pos_throttle
float64 cluster_odometer
float64 lon_acc_speed
uint8 vehicle_speed_engine
float64 yaw_rate 



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

")



(provide :can/kusv_CanInfo "0d05be062e80361fb3d556c45a03d561")


