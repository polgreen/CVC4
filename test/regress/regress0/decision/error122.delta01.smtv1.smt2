(set-option :incremental false)
(set-info :status sat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 16))
(declare-fun a2 () (Array (_ BitVec 16) (_ BitVec 7)))
(check-sat-assuming ( (not (distinct v0 ((_ sign_extend 15) (ite (bvult (_ bv0 7) (select (store a2 v0 (_ bv0 7)) (_ bv1 16))) (_ bv1 1) (_ bv0 1))))) ))