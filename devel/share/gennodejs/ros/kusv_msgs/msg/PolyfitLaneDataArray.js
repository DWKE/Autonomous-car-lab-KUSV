// Auto-generated. Do not edit!

// (in-package kusv_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let PolyfitLaneData = require('./PolyfitLaneData.js');

//-----------------------------------------------------------

class PolyfitLaneDataArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.polyfitLanes = null;
    }
    else {
      if (initObj.hasOwnProperty('polyfitLanes')) {
        this.polyfitLanes = initObj.polyfitLanes
      }
      else {
        this.polyfitLanes = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PolyfitLaneDataArray
    // Serialize message field [polyfitLanes]
    // Serialize the length for message field [polyfitLanes]
    bufferOffset = _serializer.uint32(obj.polyfitLanes.length, buffer, bufferOffset);
    obj.polyfitLanes.forEach((val) => {
      bufferOffset = PolyfitLaneData.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PolyfitLaneDataArray
    let len;
    let data = new PolyfitLaneDataArray(null);
    // Deserialize message field [polyfitLanes]
    // Deserialize array length for message field [polyfitLanes]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.polyfitLanes = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.polyfitLanes[i] = PolyfitLaneData.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 32 * object.polyfitLanes.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'kusv_msgs/PolyfitLaneDataArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f73b0014879f8495deb3d40eb300c75a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    PolyfitLaneData[] polyfitLanes
    
    ================================================================================
    MSG: kusv_msgs/PolyfitLaneData
    float64 a
    float64 b
    float64 c
    float64 d
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PolyfitLaneDataArray(null);
    if (msg.polyfitLanes !== undefined) {
      resolved.polyfitLanes = new Array(msg.polyfitLanes.length);
      for (let i = 0; i < resolved.polyfitLanes.length; ++i) {
        resolved.polyfitLanes[i] = PolyfitLaneData.Resolve(msg.polyfitLanes[i]);
      }
    }
    else {
      resolved.polyfitLanes = []
    }

    return resolved;
    }
};

module.exports = PolyfitLaneDataArray;
