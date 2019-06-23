;; Auto-generated. Do not edit!


(when (boundp 'kusv_msgs::PolyfitLaneData)
  (if (not (find-package "KUSV_MSGS"))
    (make-package "KUSV_MSGS"))
  (shadow 'PolyfitLaneData (find-package "KUSV_MSGS")))
(unless (find-package "KUSV_MSGS::POLYFITLANEDATA")
  (make-package "KUSV_MSGS::POLYFITLANEDATA"))

(in-package "ROS")
;;//! \htmlinclude PolyfitLaneData.msg.html


(defclass kusv_msgs::PolyfitLaneData
  :super ros::object
  :slots (_id _a _b _c _d ))

(defmethod kusv_msgs::PolyfitLaneData
  (:init
   (&key
    ((:id __id) "")
    ((:a __a) 0.0)
    ((:b __b) 0.0)
    ((:c __c) 0.0)
    ((:d __d) 0.0)
    )
   (send-super :init)
   (setq _id (string __id))
   (setq _a (float __a))
   (setq _b (float __b))
   (setq _c (float __c))
   (setq _d (float __d))
   self)
  (:id
   (&optional __id)
   (if __id (setq _id __id)) _id)
  (:a
   (&optional __a)
   (if __a (setq _a __a)) _a)
  (:b
   (&optional __b)
   (if __b (setq _b __b)) _b)
  (:c
   (&optional __c)
   (if __c (setq _c __c)) _c)
  (:d
   (&optional __d)
   (if __d (setq _d __d)) _d)
  (:serialization-length
   ()
   (+
    ;; string _id
    4 (length _id)
    ;; float64 _a
    8
    ;; float64 _b
    8
    ;; float64 _c
    8
    ;; float64 _d
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _id
       (write-long (length _id) s) (princ _id s)
     ;; float64 _a
       (sys::poke _a (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _b
       (sys::poke _b (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _c
       (sys::poke _c (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _d
       (sys::poke _d (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _id
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _id (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float64 _a
     (setq _a (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _b
     (setq _b (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _c
     (setq _c (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _d
     (setq _d (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get kusv_msgs::PolyfitLaneData :md5sum-) "47b0d2040cd914e7cb8c724f2341ef90")
(setf (get kusv_msgs::PolyfitLaneData :datatype-) "kusv_msgs/PolyfitLaneData")
(setf (get kusv_msgs::PolyfitLaneData :definition-)
      "string id
float64 a
float64 b
float64 c
float64 d

")



(provide :kusv_msgs/PolyfitLaneData "47b0d2040cd914e7cb8c724f2341ef90")


