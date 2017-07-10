; COMMAND-LINE: --nl-ext-tplanes
; EXPECT: unsat
(set-logic ALL)
(set-info :status unsat)
(declare-fun X () Real)
(declare-fun |sqrt{10}| () Real)
(declare-fun |sqrt{8}| () Real)
(assert (let ((.def_48 (<= X (to_real 300))))
(let ((.def_47 (<= (to_real (- 300)) X)))
(let ((.def_49 (and .def_47 .def_48)))
(let ((.def_44 (<= |sqrt{10}| (to_real 300))))
(let ((.def_43 (<= (to_real (- 300)) |sqrt{10}|)))
(let ((.def_45 (and .def_43 .def_44)))
(let ((.def_40 (<= |sqrt{8}| (to_real 300))))
(let ((.def_39 (<= (to_real (- 300)) |sqrt{8}|)))
(let ((.def_41 (and .def_39 .def_40)))
(let ((.def_35 (<= (to_real 0) |sqrt{8}|)))
(let ((.def_31 (* |sqrt{8}| |sqrt{8}|)))
(let ((.def_33 (= .def_31 (to_real 3))))
(let ((.def_29 (<= (to_real 0) |sqrt{10}|)))
(let ((.def_26 (* |sqrt{10}| |sqrt{10}|)))
(let ((.def_27 (= X .def_26)))
(let ((.def_21 (<= X (to_real 2))))
(let ((.def_19 (<= (to_real 0) X)))
(let ((.def_22 (and .def_19 .def_21)))
(let ((.def_23 (not .def_22)))
(let ((.def_11 (+ |sqrt{10}| |sqrt{8}|)))
(let ((.def_15 (<= (/ 63 20) .def_11)))
(let ((.def_16 (not .def_15)))
(let ((.def_24 (or .def_16 .def_23)))
(let ((.def_25 (not .def_24)))
(let ((.def_28 (and .def_25 .def_27)))
(let ((.def_30 (and .def_28 .def_29)))
(let ((.def_34 (and .def_30 .def_33)))
(let ((.def_36 (and .def_34 .def_35)))
(let ((.def_42 (and .def_36 .def_41)))
(let ((.def_46 (and .def_42 .def_45)))
(let ((.def_50 (and .def_46 .def_49)))
.def_50))))))))))))))))))))))))))))))))
(check-sat)
(exit)
