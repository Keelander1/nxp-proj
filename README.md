# NXP-AES-Project 
A Project of three Master Students at the university of applied sciences in Landshut Germany.

## 1.1 Introduction
The goal of the project is the construction and programming of a model vehicle for the Next Experience (NXP) Cup with a specially built, self-driving vehicle. In this competition the 1:18 scale vehicles must drive safely and independently through a course in the shortest possible time. The roadway is bordered by two black stripes applied to a white background. The vehicle must recognize these limitations and, based on their evaluation, adjust the speed of the vehicle and its steering angle. The car is driven
by two brushless direct current motors (BLDC motors). The steering is realized with a servo drive.


## 1.2 Konzept
Zu Beginn soll das Fahrzeug zuerst mit dem Standardbausatz zusammengebaut werden.
Die Software wird aus bereits vorhandenen, vorherigen Projekten zusammengesetzt und optimiert.
Ein groÿes Augenmerk liegt dabei auf einem übersichtlicheren Aufbau des Programms
und besserer Nachvollziehbarkeit durch Kommentation und eingängigerer Benennung der
Funktionen und Parameter.
Die Programmierung soll Stück für Stück vorgenommen werden. Zu Beginn wird die Ansteuerung
der BLDC-Motoren bearbeitet. Ist der Punkt erreicht, an dem die Motoren angesteuert
und die Drehzahl über die Puls-Weiten-Modulation variiert werden kann, soll eine
Möglichkeit der Drehzahlerkennung erarbeitet werden. Im Anschluss an die Inbetriebnahme
der Antriebe wird die Software für den Servomotor der Lenkung erstellt. Die Bedienung des
Fahrzeugs soll über ein Display und einen Dreh-Encoder realisiert werden. Zur Streckenerkennung
wird eine Zeilenkamera verwendet. Bei Bedarf kann zusätzlich oder ersatzweise eine
gröÿere Kamera eingesetzt werden. Nach der Inbetriebnahme der Einzelkomponenten werden
deren Ansteuerung und Auswertung mithilfe einer Regelung verknüpft. Die Software des
Fahrzeugs soll am Ende so optimiert werden, dass es schnellstmöglich, aber auch sicher durch
den Parcours fährt.
Nach der erfolgreichen Entwicklung des Fahrzeugs für das Durchfahren des Parcours soll
für eine zusätzliche Wettbewerbsdisziplin eine Objektdetektion mithilfe eines Ultraschallboards
realisiert werden, welches bereits in den vorherigen Semestern von anderen Studie
