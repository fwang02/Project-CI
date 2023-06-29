# Project-CI
Final project of Computer Interfacing (CI), FIB-UPC

## Information
- Project name: **Slider**
- Authors: Bru and Feiyang

## Introduction
CAT:

L’estructura del nostre codi comença per tota la configuració necessària (GLCD, botons, pwm, interrupcions…) i la declaració de totes les variables globals (integrals, booleans, structs, etc).


La part més important del nostre programa es basa essencialment en la crida de les funcions que hem trobat necessàries dins del while(1). Aquestes es tracten de totes les que imprimeixen/esborren elements no estàtics a la pantalla GLCD (murs, diners, fantasmes i ninot), la que detecta les col·lisions entre el ninot i tots els objectes, i la que regula la dificultat del videojoc. 


Més enllà de les funcions més bàsiques,  també hem hagut d’implementar-ne algunes per a ajudar a millorar la fluïdesa del joc, així com la de restar vides, actualitzar les barres de progrès o retornar el personatge a l’esquerra de la pantalla quan aquest col·lisionava.


Els principals problemes que ens hem trobat els hem pogut solucionar o hem trobat maneres per evitar-los. El que més hem notat ha sigut que des de la terminal no es podia moure el ninot amb massa rapidesa i deixava un cert temps entre desplaçaments. Segurament això passa perquè el programa es col·lapsa quan rep moltes interrupcions des del teclat.
També hem vist que depenent de com organitzàvem el nostre codi ens trobàvem amb certs errors degut a rebots (sobretot a les barres de progrès, segurament degut als SetDot). 
Un altre problema que ens hem trobat ha sigut en com fer que el ninot fos invencible durant un petit període de temps després de xocar contra un mur o un fantasma. Ho hem solucionat utilitzant una variable i un timer que contava segons, deixant que passin 2 segons des que xoca i desactivant la funció de col·lisió durant aquest període de temps.