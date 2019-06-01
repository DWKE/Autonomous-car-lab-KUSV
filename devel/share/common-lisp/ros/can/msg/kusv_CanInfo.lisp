; Auto-generated. Do not edit!


(cl:in-package can-msg)


;//! \htmlinclude kusv_CanInfo.msg.html

(cl:defclass <kusv_CanInfo> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (speedfr
    :reader speedfr
    :initarg :speedfr
    :type cl:float
    :initform 0.0)
   (speedfl
    :reader speedfl
    :initarg :speedfl
    :type cl:float
    :initform 0.0)
   (speedrr
    :reader speedrr
    :initarg :speedrr
    :type cl:float
    :initform 0.0)
   (speedrl
    :reader speedrl
    :initarg :speedrl
    :type cl:float
    :initform 0.0)
   (speed_avr_r
    :reader speed_avr_r
    :initarg :speed_avr_r
    :type cl:float
    :initform 0.0)
   (lat_acc_speed
    :reader lat_acc_speed
    :initarg :lat_acc_speed
    :type cl:float
    :initform 0.0)
   (parking_brake_on
    :reader parking_brake_on
    :initarg :parking_brake_on
    :type cl:fixnum
    :initform 0)
   (air_cond_on
    :reader air_cond_on
    :initarg :air_cond_on
    :type cl:fixnum
    :initform 0)
   (steering_wheel_angle
    :reader steering_wheel_angle
    :initarg :steering_wheel_angle
    :type cl:float
    :initform 0.0)
   (steering_wheel_angular
    :reader steering_wheel_angular
    :initarg :steering_wheel_angular
    :type cl:fixnum
    :initform 0)
   (steering_wheel_tq
    :reader steering_wheel_tq
    :initarg :steering_wheel_tq
    :type cl:float
    :initform 0.0)
   (pos_acc_pedal
    :reader pos_acc_pedal
    :initarg :pos_acc_pedal
    :type cl:float
    :initform 0.0)
   (brake_active_state
    :reader brake_active_state
    :initarg :brake_active_state
    :type cl:fixnum
    :initform 0)
   (brake_cylinder_press
    :reader brake_cylinder_press
    :initarg :brake_cylinder_press
    :type cl:float
    :initform 0.0)
   (engine_speed
    :reader engine_speed
    :initarg :engine_speed
    :type cl:float
    :initform 0.0)
   (gear_trans_state
    :reader gear_trans_state
    :initarg :gear_trans_state
    :type cl:fixnum
    :initform 0)
   (gear_seldisp
    :reader gear_seldisp
    :initarg :gear_seldisp
    :type cl:fixnum
    :initform 0)
   (pos_throttle
    :reader pos_throttle
    :initarg :pos_throttle
    :type cl:float
    :initform 0.0)
   (cluster_odometer
    :reader cluster_odometer
    :initarg :cluster_odometer
    :type cl:float
    :initform 0.0)
   (lon_acc_speed
    :reader lon_acc_speed
    :initarg :lon_acc_speed
    :type cl:float
    :initform 0.0)
   (vehicle_speed_engine
    :reader vehicle_speed_engine
    :initarg :vehicle_speed_engine
    :type cl:fixnum
    :initform 0)
   (yaw_rate
    :reader yaw_rate
    :initarg :yaw_rate
    :type cl:float
    :initform 0.0))
)

(cl:defclass kusv_CanInfo (<kusv_CanInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <kusv_CanInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'kusv_CanInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name can-msg:<kusv_CanInfo> is deprecated: use can-msg:kusv_CanInfo instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:header-val is deprecated.  Use can-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'speedfr-val :lambda-list '(m))
(cl:defmethod speedfr-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:speedfr-val is deprecated.  Use can-msg:speedfr instead.")
  (speedfr m))

(cl:ensure-generic-function 'speedfl-val :lambda-list '(m))
(cl:defmethod speedfl-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:speedfl-val is deprecated.  Use can-msg:speedfl instead.")
  (speedfl m))

(cl:ensure-generic-function 'speedrr-val :lambda-list '(m))
(cl:defmethod speedrr-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:speedrr-val is deprecated.  Use can-msg:speedrr instead.")
  (speedrr m))

(cl:ensure-generic-function 'speedrl-val :lambda-list '(m))
(cl:defmethod speedrl-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:speedrl-val is deprecated.  Use can-msg:speedrl instead.")
  (speedrl m))

(cl:ensure-generic-function 'speed_avr_r-val :lambda-list '(m))
(cl:defmethod speed_avr_r-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:speed_avr_r-val is deprecated.  Use can-msg:speed_avr_r instead.")
  (speed_avr_r m))

(cl:ensure-generic-function 'lat_acc_speed-val :lambda-list '(m))
(cl:defmethod lat_acc_speed-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:lat_acc_speed-val is deprecated.  Use can-msg:lat_acc_speed instead.")
  (lat_acc_speed m))

(cl:ensure-generic-function 'parking_brake_on-val :lambda-list '(m))
(cl:defmethod parking_brake_on-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:parking_brake_on-val is deprecated.  Use can-msg:parking_brake_on instead.")
  (parking_brake_on m))

(cl:ensure-generic-function 'air_cond_on-val :lambda-list '(m))
(cl:defmethod air_cond_on-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:air_cond_on-val is deprecated.  Use can-msg:air_cond_on instead.")
  (air_cond_on m))

(cl:ensure-generic-function 'steering_wheel_angle-val :lambda-list '(m))
(cl:defmethod steering_wheel_angle-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:steering_wheel_angle-val is deprecated.  Use can-msg:steering_wheel_angle instead.")
  (steering_wheel_angle m))

(cl:ensure-generic-function 'steering_wheel_angular-val :lambda-list '(m))
(cl:defmethod steering_wheel_angular-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:steering_wheel_angular-val is deprecated.  Use can-msg:steering_wheel_angular instead.")
  (steering_wheel_angular m))

(cl:ensure-generic-function 'steering_wheel_tq-val :lambda-list '(m))
(cl:defmethod steering_wheel_tq-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:steering_wheel_tq-val is deprecated.  Use can-msg:steering_wheel_tq instead.")
  (steering_wheel_tq m))

(cl:ensure-generic-function 'pos_acc_pedal-val :lambda-list '(m))
(cl:defmethod pos_acc_pedal-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:pos_acc_pedal-val is deprecated.  Use can-msg:pos_acc_pedal instead.")
  (pos_acc_pedal m))

(cl:ensure-generic-function 'brake_active_state-val :lambda-list '(m))
(cl:defmethod brake_active_state-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:brake_active_state-val is deprecated.  Use can-msg:brake_active_state instead.")
  (brake_active_state m))

(cl:ensure-generic-function 'brake_cylinder_press-val :lambda-list '(m))
(cl:defmethod brake_cylinder_press-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:brake_cylinder_press-val is deprecated.  Use can-msg:brake_cylinder_press instead.")
  (brake_cylinder_press m))

(cl:ensure-generic-function 'engine_speed-val :lambda-list '(m))
(cl:defmethod engine_speed-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:engine_speed-val is deprecated.  Use can-msg:engine_speed instead.")
  (engine_speed m))

(cl:ensure-generic-function 'gear_trans_state-val :lambda-list '(m))
(cl:defmethod gear_trans_state-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:gear_trans_state-val is deprecated.  Use can-msg:gear_trans_state instead.")
  (gear_trans_state m))

(cl:ensure-generic-function 'gear_seldisp-val :lambda-list '(m))
(cl:defmethod gear_seldisp-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:gear_seldisp-val is deprecated.  Use can-msg:gear_seldisp instead.")
  (gear_seldisp m))

(cl:ensure-generic-function 'pos_throttle-val :lambda-list '(m))
(cl:defmethod pos_throttle-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:pos_throttle-val is deprecated.  Use can-msg:pos_throttle instead.")
  (pos_throttle m))

(cl:ensure-generic-function 'cluster_odometer-val :lambda-list '(m))
(cl:defmethod cluster_odometer-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:cluster_odometer-val is deprecated.  Use can-msg:cluster_odometer instead.")
  (cluster_odometer m))

(cl:ensure-generic-function 'lon_acc_speed-val :lambda-list '(m))
(cl:defmethod lon_acc_speed-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:lon_acc_speed-val is deprecated.  Use can-msg:lon_acc_speed instead.")
  (lon_acc_speed m))

(cl:ensure-generic-function 'vehicle_speed_engine-val :lambda-list '(m))
(cl:defmethod vehicle_speed_engine-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:vehicle_speed_engine-val is deprecated.  Use can-msg:vehicle_speed_engine instead.")
  (vehicle_speed_engine m))

(cl:ensure-generic-function 'yaw_rate-val :lambda-list '(m))
(cl:defmethod yaw_rate-val ((m <kusv_CanInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:yaw_rate-val is deprecated.  Use can-msg:yaw_rate instead.")
  (yaw_rate m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <kusv_CanInfo>) ostream)
  "Serializes a message object of type '<kusv_CanInfo>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'speedfr))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'speedfl))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'speedrr))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'speedrl))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'speed_avr_r))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'lat_acc_speed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'parking_brake_on)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'air_cond_on)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'steering_wheel_angle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'steering_wheel_angular)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'steering_wheel_angular)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'steering_wheel_tq))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos_acc_pedal))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'brake_active_state)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'brake_cylinder_press))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'engine_speed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gear_trans_state)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gear_seldisp)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos_throttle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'cluster_odometer))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'lon_acc_speed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'vehicle_speed_engine)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'yaw_rate))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <kusv_CanInfo>) istream)
  "Deserializes a message object of type '<kusv_CanInfo>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speedfr) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speedfl) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speedrr) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speedrl) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speed_avr_r) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'lat_acc_speed) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'parking_brake_on)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'air_cond_on)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steering_wheel_angle) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'steering_wheel_angular)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'steering_wheel_angular)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steering_wheel_tq) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_acc_pedal) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'brake_active_state)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'brake_cylinder_press) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'engine_speed) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gear_trans_state)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gear_seldisp)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_throttle) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'cluster_odometer) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'lon_acc_speed) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'vehicle_speed_engine)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw_rate) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<kusv_CanInfo>)))
  "Returns string type for a message object of type '<kusv_CanInfo>"
  "can/kusv_CanInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'kusv_CanInfo)))
  "Returns string type for a message object of type 'kusv_CanInfo"
  "can/kusv_CanInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<kusv_CanInfo>)))
  "Returns md5sum for a message object of type '<kusv_CanInfo>"
  "0d05be062e80361fb3d556c45a03d561")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'kusv_CanInfo)))
  "Returns md5sum for a message object of type 'kusv_CanInfo"
  "0d05be062e80361fb3d556c45a03d561")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<kusv_CanInfo>)))
  "Returns full string definition for message of type '<kusv_CanInfo>"
  (cl:format cl:nil "Header header~%float64 speedfr~%float64 speedfl~%float64 speedrr~%float64 speedrl~%float64 speed_avr_r~%float64 lat_acc_speed~%uint8 parking_brake_on~%uint8 air_cond_on~%float64 steering_wheel_angle~%uint16 steering_wheel_angular~%float64 steering_wheel_tq~%float64 pos_acc_pedal~%uint8 brake_active_state~%float64 brake_cylinder_press~%float64 engine_speed~%uint8 gear_trans_state~%uint8 gear_seldisp~%float64 pos_throttle~%float64 cluster_odometer~%float64 lon_acc_speed~%uint8 vehicle_speed_engine~%float64 yaw_rate ~%~%~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'kusv_CanInfo)))
  "Returns full string definition for message of type 'kusv_CanInfo"
  (cl:format cl:nil "Header header~%float64 speedfr~%float64 speedfl~%float64 speedrr~%float64 speedrl~%float64 speed_avr_r~%float64 lat_acc_speed~%uint8 parking_brake_on~%uint8 air_cond_on~%float64 steering_wheel_angle~%uint16 steering_wheel_angular~%float64 steering_wheel_tq~%float64 pos_acc_pedal~%uint8 brake_active_state~%float64 brake_cylinder_press~%float64 engine_speed~%uint8 gear_trans_state~%uint8 gear_seldisp~%float64 pos_throttle~%float64 cluster_odometer~%float64 lon_acc_speed~%uint8 vehicle_speed_engine~%float64 yaw_rate ~%~%~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <kusv_CanInfo>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     8
     8
     8
     8
     8
     8
     1
     1
     8
     2
     8
     8
     1
     8
     8
     1
     1
     8
     8
     8
     1
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <kusv_CanInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'kusv_CanInfo
    (cl:cons ':header (header msg))
    (cl:cons ':speedfr (speedfr msg))
    (cl:cons ':speedfl (speedfl msg))
    (cl:cons ':speedrr (speedrr msg))
    (cl:cons ':speedrl (speedrl msg))
    (cl:cons ':speed_avr_r (speed_avr_r msg))
    (cl:cons ':lat_acc_speed (lat_acc_speed msg))
    (cl:cons ':parking_brake_on (parking_brake_on msg))
    (cl:cons ':air_cond_on (air_cond_on msg))
    (cl:cons ':steering_wheel_angle (steering_wheel_angle msg))
    (cl:cons ':steering_wheel_angular (steering_wheel_angular msg))
    (cl:cons ':steering_wheel_tq (steering_wheel_tq msg))
    (cl:cons ':pos_acc_pedal (pos_acc_pedal msg))
    (cl:cons ':brake_active_state (brake_active_state msg))
    (cl:cons ':brake_cylinder_press (brake_cylinder_press msg))
    (cl:cons ':engine_speed (engine_speed msg))
    (cl:cons ':gear_trans_state (gear_trans_state msg))
    (cl:cons ':gear_seldisp (gear_seldisp msg))
    (cl:cons ':pos_throttle (pos_throttle msg))
    (cl:cons ':cluster_odometer (cluster_odometer msg))
    (cl:cons ':lon_acc_speed (lon_acc_speed msg))
    (cl:cons ':vehicle_speed_engine (vehicle_speed_engine msg))
    (cl:cons ':yaw_rate (yaw_rate msg))
))
