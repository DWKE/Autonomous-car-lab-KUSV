; Auto-generated. Do not edit!


(cl:in-package kusv_msgs-msg)


;//! \htmlinclude ControlCmd.msg.html

(cl:defclass <ControlCmd> (roslisp-msg-protocol:ros-message)
  ((linear_velocity
    :reader linear_velocity
    :initarg :linear_velocity
    :type cl:float
    :initform 0.0)
   (steering_angle
    :reader steering_angle
    :initarg :steering_angle
    :type cl:float
    :initform 0.0))
)

(cl:defclass ControlCmd (<ControlCmd>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ControlCmd>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ControlCmd)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kusv_msgs-msg:<ControlCmd> is deprecated: use kusv_msgs-msg:ControlCmd instead.")))

(cl:ensure-generic-function 'linear_velocity-val :lambda-list '(m))
(cl:defmethod linear_velocity-val ((m <ControlCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kusv_msgs-msg:linear_velocity-val is deprecated.  Use kusv_msgs-msg:linear_velocity instead.")
  (linear_velocity m))

(cl:ensure-generic-function 'steering_angle-val :lambda-list '(m))
(cl:defmethod steering_angle-val ((m <ControlCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kusv_msgs-msg:steering_angle-val is deprecated.  Use kusv_msgs-msg:steering_angle instead.")
  (steering_angle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ControlCmd>) ostream)
  "Serializes a message object of type '<ControlCmd>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'linear_velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'steering_angle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ControlCmd>) istream)
  "Deserializes a message object of type '<ControlCmd>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'linear_velocity) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steering_angle) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ControlCmd>)))
  "Returns string type for a message object of type '<ControlCmd>"
  "kusv_msgs/ControlCmd")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ControlCmd)))
  "Returns string type for a message object of type 'ControlCmd"
  "kusv_msgs/ControlCmd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ControlCmd>)))
  "Returns md5sum for a message object of type '<ControlCmd>"
  "6f32152926ab6255084d804f9e199d25")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ControlCmd)))
  "Returns md5sum for a message object of type 'ControlCmd"
  "6f32152926ab6255084d804f9e199d25")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ControlCmd>)))
  "Returns full string definition for message of type '<ControlCmd>"
  (cl:format cl:nil "float64 linear_velocity~%float64 steering_angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ControlCmd)))
  "Returns full string definition for message of type 'ControlCmd"
  (cl:format cl:nil "float64 linear_velocity~%float64 steering_angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ControlCmd>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ControlCmd>))
  "Converts a ROS message object to a list"
  (cl:list 'ControlCmd
    (cl:cons ':linear_velocity (linear_velocity msg))
    (cl:cons ':steering_angle (steering_angle msg))
))
