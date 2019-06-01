// Auto-generated. Do not edit!

// (in-package can.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class kusv_CanInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.speedfr = null;
      this.speedfl = null;
      this.speedrr = null;
      this.speedrl = null;
      this.speed_avr_r = null;
      this.lat_acc_speed = null;
      this.parking_brake_on = null;
      this.air_cond_on = null;
      this.steering_wheel_angle = null;
      this.steering_wheel_angular = null;
      this.steering_wheel_tq = null;
      this.pos_acc_pedal = null;
      this.brake_active_state = null;
      this.brake_cylinder_press = null;
      this.engine_speed = null;
      this.gear_trans_state = null;
      this.gear_seldisp = null;
      this.pos_throttle = null;
      this.cluster_odometer = null;
      this.lon_acc_speed = null;
      this.vehicle_speed_engine = null;
      this.yaw_rate = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('speedfr')) {
        this.speedfr = initObj.speedfr
      }
      else {
        this.speedfr = 0.0;
      }
      if (initObj.hasOwnProperty('speedfl')) {
        this.speedfl = initObj.speedfl
      }
      else {
        this.speedfl = 0.0;
      }
      if (initObj.hasOwnProperty('speedrr')) {
        this.speedrr = initObj.speedrr
      }
      else {
        this.speedrr = 0.0;
      }
      if (initObj.hasOwnProperty('speedrl')) {
        this.speedrl = initObj.speedrl
      }
      else {
        this.speedrl = 0.0;
      }
      if (initObj.hasOwnProperty('speed_avr_r')) {
        this.speed_avr_r = initObj.speed_avr_r
      }
      else {
        this.speed_avr_r = 0.0;
      }
      if (initObj.hasOwnProperty('lat_acc_speed')) {
        this.lat_acc_speed = initObj.lat_acc_speed
      }
      else {
        this.lat_acc_speed = 0.0;
      }
      if (initObj.hasOwnProperty('parking_brake_on')) {
        this.parking_brake_on = initObj.parking_brake_on
      }
      else {
        this.parking_brake_on = 0;
      }
      if (initObj.hasOwnProperty('air_cond_on')) {
        this.air_cond_on = initObj.air_cond_on
      }
      else {
        this.air_cond_on = 0;
      }
      if (initObj.hasOwnProperty('steering_wheel_angle')) {
        this.steering_wheel_angle = initObj.steering_wheel_angle
      }
      else {
        this.steering_wheel_angle = 0.0;
      }
      if (initObj.hasOwnProperty('steering_wheel_angular')) {
        this.steering_wheel_angular = initObj.steering_wheel_angular
      }
      else {
        this.steering_wheel_angular = 0;
      }
      if (initObj.hasOwnProperty('steering_wheel_tq')) {
        this.steering_wheel_tq = initObj.steering_wheel_tq
      }
      else {
        this.steering_wheel_tq = 0.0;
      }
      if (initObj.hasOwnProperty('pos_acc_pedal')) {
        this.pos_acc_pedal = initObj.pos_acc_pedal
      }
      else {
        this.pos_acc_pedal = 0.0;
      }
      if (initObj.hasOwnProperty('brake_active_state')) {
        this.brake_active_state = initObj.brake_active_state
      }
      else {
        this.brake_active_state = 0;
      }
      if (initObj.hasOwnProperty('brake_cylinder_press')) {
        this.brake_cylinder_press = initObj.brake_cylinder_press
      }
      else {
        this.brake_cylinder_press = 0.0;
      }
      if (initObj.hasOwnProperty('engine_speed')) {
        this.engine_speed = initObj.engine_speed
      }
      else {
        this.engine_speed = 0.0;
      }
      if (initObj.hasOwnProperty('gear_trans_state')) {
        this.gear_trans_state = initObj.gear_trans_state
      }
      else {
        this.gear_trans_state = 0;
      }
      if (initObj.hasOwnProperty('gear_seldisp')) {
        this.gear_seldisp = initObj.gear_seldisp
      }
      else {
        this.gear_seldisp = 0;
      }
      if (initObj.hasOwnProperty('pos_throttle')) {
        this.pos_throttle = initObj.pos_throttle
      }
      else {
        this.pos_throttle = 0.0;
      }
      if (initObj.hasOwnProperty('cluster_odometer')) {
        this.cluster_odometer = initObj.cluster_odometer
      }
      else {
        this.cluster_odometer = 0.0;
      }
      if (initObj.hasOwnProperty('lon_acc_speed')) {
        this.lon_acc_speed = initObj.lon_acc_speed
      }
      else {
        this.lon_acc_speed = 0.0;
      }
      if (initObj.hasOwnProperty('vehicle_speed_engine')) {
        this.vehicle_speed_engine = initObj.vehicle_speed_engine
      }
      else {
        this.vehicle_speed_engine = 0;
      }
      if (initObj.hasOwnProperty('yaw_rate')) {
        this.yaw_rate = initObj.yaw_rate
      }
      else {
        this.yaw_rate = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type kusv_CanInfo
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [speedfr]
    bufferOffset = _serializer.float64(obj.speedfr, buffer, bufferOffset);
    // Serialize message field [speedfl]
    bufferOffset = _serializer.float64(obj.speedfl, buffer, bufferOffset);
    // Serialize message field [speedrr]
    bufferOffset = _serializer.float64(obj.speedrr, buffer, bufferOffset);
    // Serialize message field [speedrl]
    bufferOffset = _serializer.float64(obj.speedrl, buffer, bufferOffset);
    // Serialize message field [speed_avr_r]
    bufferOffset = _serializer.float64(obj.speed_avr_r, buffer, bufferOffset);
    // Serialize message field [lat_acc_speed]
    bufferOffset = _serializer.float64(obj.lat_acc_speed, buffer, bufferOffset);
    // Serialize message field [parking_brake_on]
    bufferOffset = _serializer.uint8(obj.parking_brake_on, buffer, bufferOffset);
    // Serialize message field [air_cond_on]
    bufferOffset = _serializer.uint8(obj.air_cond_on, buffer, bufferOffset);
    // Serialize message field [steering_wheel_angle]
    bufferOffset = _serializer.float64(obj.steering_wheel_angle, buffer, bufferOffset);
    // Serialize message field [steering_wheel_angular]
    bufferOffset = _serializer.uint16(obj.steering_wheel_angular, buffer, bufferOffset);
    // Serialize message field [steering_wheel_tq]
    bufferOffset = _serializer.float64(obj.steering_wheel_tq, buffer, bufferOffset);
    // Serialize message field [pos_acc_pedal]
    bufferOffset = _serializer.float64(obj.pos_acc_pedal, buffer, bufferOffset);
    // Serialize message field [brake_active_state]
    bufferOffset = _serializer.uint8(obj.brake_active_state, buffer, bufferOffset);
    // Serialize message field [brake_cylinder_press]
    bufferOffset = _serializer.float64(obj.brake_cylinder_press, buffer, bufferOffset);
    // Serialize message field [engine_speed]
    bufferOffset = _serializer.float64(obj.engine_speed, buffer, bufferOffset);
    // Serialize message field [gear_trans_state]
    bufferOffset = _serializer.uint8(obj.gear_trans_state, buffer, bufferOffset);
    // Serialize message field [gear_seldisp]
    bufferOffset = _serializer.uint8(obj.gear_seldisp, buffer, bufferOffset);
    // Serialize message field [pos_throttle]
    bufferOffset = _serializer.float64(obj.pos_throttle, buffer, bufferOffset);
    // Serialize message field [cluster_odometer]
    bufferOffset = _serializer.float64(obj.cluster_odometer, buffer, bufferOffset);
    // Serialize message field [lon_acc_speed]
    bufferOffset = _serializer.float64(obj.lon_acc_speed, buffer, bufferOffset);
    // Serialize message field [vehicle_speed_engine]
    bufferOffset = _serializer.uint8(obj.vehicle_speed_engine, buffer, bufferOffset);
    // Serialize message field [yaw_rate]
    bufferOffset = _serializer.float64(obj.yaw_rate, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type kusv_CanInfo
    let len;
    let data = new kusv_CanInfo(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [speedfr]
    data.speedfr = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [speedfl]
    data.speedfl = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [speedrr]
    data.speedrr = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [speedrl]
    data.speedrl = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [speed_avr_r]
    data.speed_avr_r = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [lat_acc_speed]
    data.lat_acc_speed = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [parking_brake_on]
    data.parking_brake_on = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [air_cond_on]
    data.air_cond_on = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [steering_wheel_angle]
    data.steering_wheel_angle = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [steering_wheel_angular]
    data.steering_wheel_angular = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [steering_wheel_tq]
    data.steering_wheel_tq = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [pos_acc_pedal]
    data.pos_acc_pedal = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [brake_active_state]
    data.brake_active_state = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [brake_cylinder_press]
    data.brake_cylinder_press = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [engine_speed]
    data.engine_speed = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [gear_trans_state]
    data.gear_trans_state = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [gear_seldisp]
    data.gear_seldisp = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [pos_throttle]
    data.pos_throttle = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [cluster_odometer]
    data.cluster_odometer = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [lon_acc_speed]
    data.lon_acc_speed = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [vehicle_speed_engine]
    data.vehicle_speed_engine = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [yaw_rate]
    data.yaw_rate = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 128;
  }

  static datatype() {
    // Returns string type for a message object
    return 'can/kusv_CanInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '0d05be062e80361fb3d556c45a03d561';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new kusv_CanInfo(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.speedfr !== undefined) {
      resolved.speedfr = msg.speedfr;
    }
    else {
      resolved.speedfr = 0.0
    }

    if (msg.speedfl !== undefined) {
      resolved.speedfl = msg.speedfl;
    }
    else {
      resolved.speedfl = 0.0
    }

    if (msg.speedrr !== undefined) {
      resolved.speedrr = msg.speedrr;
    }
    else {
      resolved.speedrr = 0.0
    }

    if (msg.speedrl !== undefined) {
      resolved.speedrl = msg.speedrl;
    }
    else {
      resolved.speedrl = 0.0
    }

    if (msg.speed_avr_r !== undefined) {
      resolved.speed_avr_r = msg.speed_avr_r;
    }
    else {
      resolved.speed_avr_r = 0.0
    }

    if (msg.lat_acc_speed !== undefined) {
      resolved.lat_acc_speed = msg.lat_acc_speed;
    }
    else {
      resolved.lat_acc_speed = 0.0
    }

    if (msg.parking_brake_on !== undefined) {
      resolved.parking_brake_on = msg.parking_brake_on;
    }
    else {
      resolved.parking_brake_on = 0
    }

    if (msg.air_cond_on !== undefined) {
      resolved.air_cond_on = msg.air_cond_on;
    }
    else {
      resolved.air_cond_on = 0
    }

    if (msg.steering_wheel_angle !== undefined) {
      resolved.steering_wheel_angle = msg.steering_wheel_angle;
    }
    else {
      resolved.steering_wheel_angle = 0.0
    }

    if (msg.steering_wheel_angular !== undefined) {
      resolved.steering_wheel_angular = msg.steering_wheel_angular;
    }
    else {
      resolved.steering_wheel_angular = 0
    }

    if (msg.steering_wheel_tq !== undefined) {
      resolved.steering_wheel_tq = msg.steering_wheel_tq;
    }
    else {
      resolved.steering_wheel_tq = 0.0
    }

    if (msg.pos_acc_pedal !== undefined) {
      resolved.pos_acc_pedal = msg.pos_acc_pedal;
    }
    else {
      resolved.pos_acc_pedal = 0.0
    }

    if (msg.brake_active_state !== undefined) {
      resolved.brake_active_state = msg.brake_active_state;
    }
    else {
      resolved.brake_active_state = 0
    }

    if (msg.brake_cylinder_press !== undefined) {
      resolved.brake_cylinder_press = msg.brake_cylinder_press;
    }
    else {
      resolved.brake_cylinder_press = 0.0
    }

    if (msg.engine_speed !== undefined) {
      resolved.engine_speed = msg.engine_speed;
    }
    else {
      resolved.engine_speed = 0.0
    }

    if (msg.gear_trans_state !== undefined) {
      resolved.gear_trans_state = msg.gear_trans_state;
    }
    else {
      resolved.gear_trans_state = 0
    }

    if (msg.gear_seldisp !== undefined) {
      resolved.gear_seldisp = msg.gear_seldisp;
    }
    else {
      resolved.gear_seldisp = 0
    }

    if (msg.pos_throttle !== undefined) {
      resolved.pos_throttle = msg.pos_throttle;
    }
    else {
      resolved.pos_throttle = 0.0
    }

    if (msg.cluster_odometer !== undefined) {
      resolved.cluster_odometer = msg.cluster_odometer;
    }
    else {
      resolved.cluster_odometer = 0.0
    }

    if (msg.lon_acc_speed !== undefined) {
      resolved.lon_acc_speed = msg.lon_acc_speed;
    }
    else {
      resolved.lon_acc_speed = 0.0
    }

    if (msg.vehicle_speed_engine !== undefined) {
      resolved.vehicle_speed_engine = msg.vehicle_speed_engine;
    }
    else {
      resolved.vehicle_speed_engine = 0
    }

    if (msg.yaw_rate !== undefined) {
      resolved.yaw_rate = msg.yaw_rate;
    }
    else {
      resolved.yaw_rate = 0.0
    }

    return resolved;
    }
};

module.exports = kusv_CanInfo;
