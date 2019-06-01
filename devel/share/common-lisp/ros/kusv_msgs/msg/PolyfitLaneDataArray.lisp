; Auto-generated. Do not edit!


(cl:in-package kusv_msgs-msg)


;//! \htmlinclude PolyfitLaneDataArray.msg.html

(cl:defclass <PolyfitLaneDataArray> (roslisp-msg-protocol:ros-message)
  ((polyfitLanes
    :reader polyfitLanes
    :initarg :polyfitLanes
    :type (cl:vector kusv_msgs-msg:PolyfitLaneData)
   :initform (cl:make-array 0 :element-type 'kusv_msgs-msg:PolyfitLaneData :initial-element (cl:make-instance 'kusv_msgs-msg:PolyfitLaneData))))
)

(cl:defclass PolyfitLaneDataArray (<PolyfitLaneDataArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PolyfitLaneDataArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PolyfitLaneDataArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kusv_msgs-msg:<PolyfitLaneDataArray> is deprecated: use kusv_msgs-msg:PolyfitLaneDataArray instead.")))

(cl:ensure-generic-function 'polyfitLanes-val :lambda-list '(m))
(cl:defmethod polyfitLanes-val ((m <PolyfitLaneDataArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kusv_msgs-msg:polyfitLanes-val is deprecated.  Use kusv_msgs-msg:polyfitLanes instead.")
  (polyfitLanes m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PolyfitLaneDataArray>) ostream)
  "Serializes a message object of type '<PolyfitLaneDataArray>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'polyfitLanes))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'polyfitLanes))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PolyfitLaneDataArray>) istream)
  "Deserializes a message object of type '<PolyfitLaneDataArray>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'polyfitLanes) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'polyfitLanes)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'kusv_msgs-msg:PolyfitLaneData))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PolyfitLaneDataArray>)))
  "Returns string type for a message object of type '<PolyfitLaneDataArray>"
  "kusv_msgs/PolyfitLaneDataArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PolyfitLaneDataArray)))
  "Returns string type for a message object of type 'PolyfitLaneDataArray"
  "kusv_msgs/PolyfitLaneDataArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PolyfitLaneDataArray>)))
  "Returns md5sum for a message object of type '<PolyfitLaneDataArray>"
  "f73b0014879f8495deb3d40eb300c75a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PolyfitLaneDataArray)))
  "Returns md5sum for a message object of type 'PolyfitLaneDataArray"
  "f73b0014879f8495deb3d40eb300c75a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PolyfitLaneDataArray>)))
  "Returns full string definition for message of type '<PolyfitLaneDataArray>"
  (cl:format cl:nil "PolyfitLaneData[] polyfitLanes~%~%================================================================================~%MSG: kusv_msgs/PolyfitLaneData~%float64 a~%float64 b~%float64 c~%float64 d~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PolyfitLaneDataArray)))
  "Returns full string definition for message of type 'PolyfitLaneDataArray"
  (cl:format cl:nil "PolyfitLaneData[] polyfitLanes~%~%================================================================================~%MSG: kusv_msgs/PolyfitLaneData~%float64 a~%float64 b~%float64 c~%float64 d~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PolyfitLaneDataArray>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'polyfitLanes) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PolyfitLaneDataArray>))
  "Converts a ROS message object to a list"
  (cl:list 'PolyfitLaneDataArray
    (cl:cons ':polyfitLanes (polyfitLanes msg))
))
