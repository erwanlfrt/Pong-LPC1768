# Pong for LPC1768

## You won't find a better Pong for LPC1768 (you won't find any other Pong game for LPC1768)

Who said that we cannot play on a LPC1768 microprocessor ? Okay this program is just a game between 2 bots but it's ten times more interesting to watch a game between two well-coded bots than a game between stupids humans. 

### I. I wanna play !
I recommand to use Keil µvision, it's definitely the best IDE for development on microprocessors. Open this project on Keil, compile and execute it. 

### II. What it does...
As I said, this pong is just a game between two bots. It generates a random start position for the ball and then bots try to kick the ball. 
#### Bot algorithm
Bot computes the exact trajectory of the ball. In order to avoid an infinite game, bots are 3 times slower than the ball. Even if the trajectory is well computed, bot may havn't enough time to reach the point. 

So movements of the ball is handled by timer 0 and movements of both players are handled by timer 1. 

### III. Possible improvements
- create a human playable version with touch-screen commands or buttons.
- implement different level of bots by changing the speed of the bot. 

