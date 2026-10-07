.org 0
     data ra, #0
_1:  oum [ra]
     inc ra
     jc #end
     jmp #_1
end: hlt
