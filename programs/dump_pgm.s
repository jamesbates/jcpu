.org 0
     data ra, #0
_1:  oup [ra]
     inc ra
     jc #end
     jmp #_1
end: hlt
