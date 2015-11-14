; stdout
(print "Hello, World!")
(println "Hello, " "World" "!")

; arithmetics
(println (+ 1 2 3 4))

; complex example
(let [actions {:hello "bonjour"}
      names   {:world "monde"}]
  (println (:hello actions) " " (:world names)))