# Spaceship-Battle
EEC172 - Spaceship Battle


 

#Top level description
We made an interactive game using the accelerometer as the main mechanic. This code is built to use the accelerometer to control the center ball. The player will control a ball and there would be another computer controlled ball. The goal is to shove the other ball out of the screen and a “you win”/”you lose” will show up on the screen and also be sent as a text message to a phone. The description of the main functions will be discussed in this section.




#2nd Ball Algorithm (Enemy ball)
The enemy ball’s movement based on the movement of the player-controlled ball. We used vector mathematics to calculate the enemy ball’s velocity. We first set its velocity to be a unit vector in the same direction as the relative position of the player’s ball. We then multiplied both the x- and y- components of the enemy ball’s velocity by a constant.


#Collision Detection (Hitbox)
A box is formed around both balls and when the box touches, we assign the post-collision velocity values to each ball, creating a collision/bounceback from each other. The code snippet for hitbox detection is shown below:



#Internet connection
Our internet connection certificate is obtained from AWS. We used an SMS system that we have made previously to send results over the internet to a phone.
