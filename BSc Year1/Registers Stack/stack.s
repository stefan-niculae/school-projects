.data

n: .byte 0
v: .space 100

nEgalCu: .asciiz "n = "
vEgalCu: .asciiz "v = "
stivaEgal: .asciiz "stiva = "

spatiu: .asciiz " "

.text

# t0 = n
# t1 = i

citire:

	li $v0, 4							# sa afiseze (string)
	la $a0, nEgalCu						# "n = "
	syscall 							# afiseaza

	li $v0, 5							# sa citeasca
	syscall 							# citeste (in v0)
	sb $v0, n 							# n = v0
	lb $t0, n 							# t0 = n

	li $t1, 0							# i = 0
	la $t2, v 							# in t2 avem adresa de inceput a lui v[]
	buclaCitire:
		beq $t1, $t0, endBuclaCitire	# if(i == n) end
		addi $t1, 1 					# i++

		li $v0, 4 						# sa afiseze (string)
		la $a0, vEgalCu					# "v = "
		syscall 						# sa afiseze

		li $v0, 5						# sa citeasca
		syscall 						# citeste (in v0)
		sb $v0, v($t1)					# v[i]

		b buclaCitire
	endBuclaCitire:

	jr $ra								# se intoarce la main

afisare:

	lb $t0, n 							# t0 = n
	li $t1, 0							# i = 0

	buclaAfisare:
		beq $t1, $t0, endBuclaAfisare 	# if(i == n) end
		addi $t1, 1 					# i++

		li $v0, 1 						# sa afiseze (int)
		lb $a0, v($t1) 					# v[i]
		syscall							# afiseaza

		li $v0, 4 						# sa afiseze (string)
		la $a0, spatiu 					# " "
		syscall 						# afiseaza

		b buclaAfisare
	endBuclaAfisare:

	jr $ra

citireStiva:
	
	li $v0, 4							# sa afiseze (string)
	la $a0, nEgalCu						# "n = "
	syscall 							# afiseaza

	li $v0, 5							# sa citeasca
	syscall 							# citeste (in v0)
	sb $v0, n 							# n = v0
	lb $t0, n 							# t0 = n

	li $t1, 0 							# i = 0
	buclaCitireSt: 						
		beq $t1, $t0, endBuclaCitireSt  # if (i == n) end

		li $v0, 4 						# afisare(string)
		la $a0, stivaEgal 				# "stiva = "
		syscall 						# afiseaza

		li $v0, 5						# citire (int)
		syscall 						# citeste (in v0)
		sub $sp, 1 						# stiva--
		sb $v0, ($sp) 					# stiva = v0

		add $t1, 1 						# i++
		b buclaCitireSt
	endBuclaCitireSt:

	jr $ra


afisareStiva:
	lb $t0, n 							# t0 = n
	li $t1, 0 							# i = 0

	buclaAfisareSt:
		beq $t1, $t0, endBuclaAfisareSt

		li $v0, 1 						# afisare (int)
		lb $a0, ($sp) 					# stiva[i]
		syscall  						# afiseaza

		li $v0, 4 						# afisare (string)
		la $a0, spatiu 					# " "
		syscall 						# afiseaza

		add $t1, 1 						# i++
		add $sp, 1 						# stiva++
		b buclaAfisareSt
	endBuclaAfisareSt:

	jr $ra

main:

	#jal citire
	#jal afisare

	jal citireStiva
	jal afisareStiva

li $v0, 10
syscall