%% Program Language : Prolog 과제
%% 컴퓨터공학과 3학년    %% 20093284 나홍철

%% 1.merge
merge(X,[], X). 
merge([],Y, Y).
merge([H1|T1],[H2|T2],[H1|L]) :- 
	H1 =< H2,
	merge(T1,[H2|T2],L),!.
merge([H1|T1],[H2|T2],[H2|L]) :- 
	H1 > H2,
	merge([H1|T1],T2,L),!.

%% 2.len
len([H|T],N) :-
    len(H, H2),
    len(T, T2),
    !,
    N is H2 + T2.
len([], 0).
len(_, 1).

%% 3.flat
flatten([],[]) :- !.
flatten([H|T],A) :- !,
 flatten(H,B), 
 flatten(T,C), 
 append(B,C,A).
flatten(H,[H]).


%% 4. predicate
%% 원소가 list에 있는지 검사하는 sub함수 %%
cmpf([H1|_],[H2|_]) :-
	H1==H2,!.
cmpf([H1|_],[H2|T]) :-
	H1\==H2,
	cmpf([H1],T).
	
%% Subset %%
subset([],_) :- !.
subset([H1|T1],B) :- 
	cmpf([H1],B),
	subset(T1,B),!.

subset(_,[]) :- !.
subset(A,[H1|T1]) :- 
	cmpf([H1],A),
	subset(A,T1),!.
	
	
%% Union %%
%% union([1,2],[1,3,4],X) 의 결과 X=[1,2,3,4]
union(X,[], X). 
union([],Y, Y).

union([H1|T1],[H2|T2],[H1|L]) :- 
	H1 == H2,
	union(T1,T2,L),!.
union([H1|T1],[H2|T2],[H1|L]) :- 
	H1 < H2,
	union(T1,[H2|T2],L),!.
union([H1|T1],[H2|T2],[H2|L]) :- 
	H1 > H2,
	union([H1|T1],T2,L),!.

	
%% Intersection %%
%% intersection([1,2],[1,3,4],X) 의 결과 X=[1]
intersection(X,[], X). 
intersection([],_, []).

intersection([H1|T1],B,[H1|L]) :- 
	cmpf([H1],B),
	intersection(T1,B,L),!.
	
intersection([H1|T1],B,L) :- 
	not(cmpf([H1],B)),
	intersection(T1,B,L),!.

	
%% set_diff %%
%% union([1,2],[1,3,4],X) 의 결과 X=[2] (인자1-인자2)
set_diff(X,[], X). 
set_diff([],_, []).

set_diff([H1|T1],B,[H1|L]) :- 
	not(cmpf([H1],B)),
	set_diff(T1,B,L),!.
	
set_diff([H1|T1],B,L) :- 
	cmpf([H1],B),
	set_diff(T1,B,L),!.

