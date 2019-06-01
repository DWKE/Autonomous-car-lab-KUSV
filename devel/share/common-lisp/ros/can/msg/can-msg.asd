
(cl:in-package :asdf)

(defsystem "can-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "kusv_CanInfo" :depends-on ("_package_kusv_CanInfo"))
    (:file "_package_kusv_CanInfo" :depends-on ("_package"))
  ))