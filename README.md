# Project-CI
Final project of Computer Interfacing (CI), FIB-UPC

## Information
- Microcontroller model: **Microchip PIC18F45K22**
- Compiler: MPLAB XC8
- Project name: **Slider**
- Simulator: Proteus Professional v8.13
- Authors: Bru and Feiyang

## Introduction
ENG:

The structure of our code starts with all the necessary configuration (GLCD, buttons, pwm, interrupts...) and the declaration of all global variables (integrals, booleans, structs, etc.).


The most important part of our programme is essentially based on the call for the functions we have found necessary within the while(1). These are all those that print/delete nonstatic elements on the GLCD screen (walls, money, ghosts and doll), which detects collisions between the doll and all objects, and which regulates the difficulty of the video game.


Beyond the most basic features, we have also had to implement some to help improve the game's fluidity, as well as subtracting lives, updating the progress bars, or returning the character to the left of the screen when the crashed.


The main problems we've encountered have been solved or we've found ways to avoid them. What we've noticed most is that the terminal couldn't move the doll too quickly and left some time between shifts. This is likely because the program collapses when it receives many interruptions from the keyboard.
We have also seen that depending on how we organized our code we found certain errors due to bounces (especially on the progress bars, probably due to the SetDot).
Another problem that we found was how to make the doll invincible for a short period of time after colliding with a wall or ghost. We solved this using a variable and a timer that counted seconds, letting it pass 2 seconds from the crash and deactivating the collision function during this time period.

CAT:

L’estructura del nostre codi comença per tota la configuració necessària (GLCD, botons, pwm, interrupcions…) i la declaració de totes les variables globals (integrals, booleans, structs, etc).


La part més important del nostre programa es basa essencialment en la crida de les funcions que hem trobat necessàries dins del while(1). Aquestes es tracten de totes les que imprimeixen/esborren elements no estàtics a la pantalla GLCD (murs, diners, fantasmes i ninot), la que detecta les col·lisions entre el ninot i tots els objectes, i la que regula la dificultat del videojoc. 


Més enllà de les funcions més bàsiques,  també hem hagut d’implementar-ne algunes per a ajudar a millorar la fluïdesa del joc, així com la de restar vides, actualitzar les barres de progrès o retornar el personatge a l’esquerra de la pantalla quan aquest col·lisionava.


Els principals problemes que ens hem trobat els hem pogut solucionar o hem trobat maneres per evitar-los. El que més hem notat ha sigut que des de la terminal no es podia moure el ninot amb massa rapidesa i deixava un cert temps entre desplaçaments. Segurament això passa perquè el programa es col·lapsa quan rep moltes interrupcions des del teclat.
També hem vist que depenent de com organitzàvem el nostre codi ens trobàvem amb certs errors degut a rebots (sobretot a les barres de progrès, segurament degut als SetDot). 
Un altre problema que ens hem trobat ha sigut en com fer que el ninot fos invencible durant un petit període de temps després de xocar contra un mur o un fantasma. Ho hem solucionat utilitzant una variable i un timer que contava segons, deixant que passin 2 segons des que xoca i desactivant la funció de col·lisió durant aquest període de temps.