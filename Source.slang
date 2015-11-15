; stdout
(print "Hello, World!")
(println "Hello, " "World" "!")

; arithmetics
(println (+ 1 2 3 4))

; complex example
(let [actions {:hello "bonjour"}
      names   {:world "monde"}]
  (println (:hello actions) " " (:world names)))

; nested scopes
(let [var 1 hello "hello"]
  (let [var 123]
    (println var)
    (println hello)))

(let [name (readln "What's your name? ")]
  (println "Hello, " name "!"))

(defun hello-world [name]
  (println "Hello, " name "!"))

(hello-world "World")

(let [name "global"]
  (defun print-name [name] (println name))
  (let [name "local"]
    (print-name name)))

(case
  (() "falsy")
  ([] "falsy")
  ({} "falsy")
  (nil "falsy")
  ("" "falsy")
  (0 "falsy")
  (:true "truthy"))

(defun bad-function []
  (raise "boom!"))

(defun good-function [] nil)

(try
  (do
    (good-function)
    (println "everyhing behave smoothly"))

  ([cause]
    (println "something bad happened: " cause)))

(try
  (do
    (bad-function)
    (println "everyhing behave smoothly"))

  ([cause]
    (println "something bad happened: " cause)))

(defmacro if [test block else]
  '(case
    (#test #block)
    (:true #else)))

(if nil   "true" "false")
(if :true "true" "false")
