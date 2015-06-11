;------------------------------------------
;  ProgramLanguage - 20093284 - 나홍철    |
;------------------------------------------

;;######################################### 1)
;--------------------------------1.1 last-item (clear)
(defun last-item (lst)
	(cond ( ( null (cdr lst)) lst)
		( t (last-item (cdr lst)))
	)
)

;--------------------------------1.2 remove-1st (clear)
(defun remove-1st (atm lst)
   (cond ( (null (cdr lst)) lst)
		 ( (not (equal atm (car lst))) (cons (car lst) (remove-1st atm (cdr lst))) )
		 ( t (cdr lst) )
   )
)

;--------------------------------1.3 append (clear) -> append2
(defun append2 (lst1 lst2)
	(cond ( (null (car lst1)) lst2) 
		( (null (cdr lst1)) (cons (car lst1) lst2) )
	    ( t  (cons (car lst1) (append (cdr lst1) lst2)) )
	)	
)

;--------------------------------1.4 merge (clear) -> merge2
(defun merge2 (lst1 lst2)
	(cond ( (null lst1) lst1)
		( (null lst2) lst2 )
		( (> (car lst1) (car lst2)) (cons (car lst2) (merge2 lst1 (cdr lst2) )) )
		( t (cons (car lst1) (merge2 (cdr lst1) lst2 )) )	
	)
	
)

;--------------------------------1.5 subst-all (clear)
(defun subst-all (x y z)
   (cond ( (null z) z)
		 ( (equal y (car z)) (cons x (subst-all x y (cdr z))) )
         ( t (cons (car z) (subst-all x y (cdr z))) )
   )
)

;######################################### 2)
;--------------------------------2.1 count-all (clear)
(defun count-all (x)
   (cond ( (null x) 0 )
         ( (atom (car x)) (1+ (count-all (cdr x))) )
         ( t (+ (count-all (car x)) (count-all (cdr x))) )
	 )
)

;--------------------------------2.2 remove-all2 (clear) 
(defun remove-all2 (lst1 lst2)
	(cond ( (null lst2) lst2) 
		( (equal lst1 (car lst2)) (remove-all2 lst1 (cdr lst2)) ) 		
		( (not (atom (car lst2))) (cons (remove-all2 lst1 (car lst2)) (remove-all2 lst1 (cdr lst2))) )
		( t (cons (car lst2) (remove-all2 lst1 (cdr lst2))) )
	)
)

;--------------------------------2.3 reverse-all (clear)
(defun reverse-all (list)
   (do ( (x list (cdr x))
         (y '() (cons (cond ( (atom(car x)) (car x) )
					  ( (not (atom (car x))) (reverse-all (car x)) ) )
					 y)) )
       ( (endp x) y )
   )
)

;--------------------------------2.4 depth (clear)
(defun depth (x)
   (cond ( (atom x) 0 )
         ( t (max (1+ (depth (car x))) (depth (cdr x))) )
   )
)

;--------------------------------2.5 flatten (clear)
(defun flatten (x)
   (cond ( (null x) x )
	 ( (atom (car x)) (cons (car x) (flatten (cdr x))) )
	 ( t (append (flatten (car x)) (flatten (cdr x))) )
   )
)

;######################################### 3)
;--------------------------------3.1 memberp (clear)
(defun memberp (atm lst)
   (cond  ( (null (cdr lst)) nil )
          ( (equal atm (car lst)) t )
   	  ( t (memberp atm (cdr lst)) )
   )
)

;--------------------------------3.2 union (clear) -> union2
(defun union2 (lst1 lst2)
	(do ( (x lst1 (cdr x))
        (y '() (cons (car x) y)) )
		( (endp x) 
			(do ( (z y (cdr z))
				(y lst2 (cons (car z) y)) )
				( (endp z) y)) )
   )
)

;--------------------------------3.3 set-diff (clear)
(defun set-diff (lst1 lst2)
	(cond ( (null lst2) lst1  )
	( t (set-diff (remove-all2 (car lst2) lst1) (cdr lst2) ) )
	)
)

;--------------------------------3.4 subsetp (clear) -> subsetp2
(defun subsetp2 (lst1 lst2)
	(cond ( (null lst1) t ) 
		( (memberp (car lst1) lst2 ) (subsetp (cdr lst1) lst2 ) )
		( t nil )
	)
)

;--------------------------------3.5 comp (clear)
(defun comp (lst1 lst2)
	(cond ( (null lst2) lst1  )
	( t (set-diff (remove-all2 (car lst2) lst1) (cdr lst2) ) )
	)
)

;######################################### 4) (clear)
;infer
(defun infer (lst1 lst2)
	(cond ( (null (memberp (car lst2) lst1)) nil)
		(t (infer1 lst1 lst2) ) )
)

;infer sub func
(defun infer1 (lst1 lst2)
	(cond ( (null lst1) nil )
		( (equal (car lst1) (car lst2)) (cond ( (null (car(cdr lst2))) (infer1 (cdr lst1) lst2) )
											(t (union (flatten (cdr lst2)) (infer1 (cdr lst1) lst2))) 
										) 
		) 
		( t (cons (car lst1) (infer1 (cdr lst1) lst2)) )	
	)
)

;######################################### 5) (clear)
;distance
(defun distance (lst1 lst2)
	(cond ( (null lst1) 0)
		( (equal 1 (- (car lst1) (car lst2))) (+ (- (car lst1) (car lst2)) (distance (cdr lst1) (cdr lst2) )) ) 
		( (equal 1 (- (car lst2) (car lst1))) (+ (- (car lst2) (car lst1)) (distance (cdr lst1) (cdr lst2) )) ) 		 
		( t (distance (cdr lst1) (cdr lst2)))
	)
) 
;closest
(defun closest (lst)
	(cond ( (null (cdr lst)) nil)
		(t (closest1 lst (list (car lst) (car(cdr lst))) ) )
	)
)
;cloesest sub func
(defun closest1 (lst tl)
	(cond ( (null (cdr lst)) tl)
		(t 
			(do ( (x (cdr lst) (cdr x))
				(y tl (cond ( ( >= (distance (car tl) (car(cdr tl))) (distance (car lst) (car x)) ) (list (car lst) (car x)) )
							(t	tl)) )
				) 
				( (endp x) (closest1 (cdr lst) y))
			)
		)
	)		
)
;-> 문제에선 (closest '((0 0 1 1) (1 0 1 1) (1 0 1 1) (0 0 1 1))) => ((1 0 1 1) (1 0 1 1)) 이 정답이라고 되어있지만
; ((0 0 1 1) (0 0 1 1))값 또한 distance값이 0이므로 정답입니다.



