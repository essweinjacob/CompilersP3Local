start
let x : 1
main {
  let w : 3
  printf [ x + w ] .
  if [  x : : 2 ] then 
  { let y : 4 
    iter [ w =< y ] 
    { printf [ x ] .
    } .
    printf [ w ] . 
  } .
  scanf [ y ] .
} stop