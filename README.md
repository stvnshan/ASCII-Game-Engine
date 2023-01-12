# ASCII-Game-Engine

To play the game, use the Makefile in each folder to compile.

Some pictures about the games:

![image](https://user-images.githubusercontent.com/83726215/211976455-294d7d30-3075-414b-b87a-de8e3c9dace0.png)

![image](https://user-images.githubusercontent.com/83726215/211976608-adc2192b-a7bf-480e-acac-a39b83563e06.png)

![image](https://user-images.githubusercontent.com/83726215/211976760-d0718eb1-e676-41e1-a874-2fc0aa2439b9.png)

![image](https://user-images.githubusercontent.com/83726215/211976871-85cd1e48-fac1-48c8-96f8-23283281bbd1.png)



Overview


  The entire engine follows MVC design structure. 

  Model has views and controllers.

  Engine class is a subclass of Model which has curseView and curseKeyboard which are subclasses of View and Controller and Engine basically control everything like 
what to print, what to generate, what happens if two objects collide. 

  Engine will ask curseKeyboard for input when input is needed, it will also tell curseView what should be printed on the screen. 

  The Engine class has a Timer and the relation between Engine and Timer follows an observer pattern, Timer will notify Engine when 0.05 second has passed and Engine 
will check whether game state is win or lose or ongoing, and if the game is still ongoing,  move each entity(registered on the board) according to their movement and 
Engine will also update the view. 

  The Engine class has a grid which contains 22*80 Cells which record the information (which objects are in this position) on each position and tell View what to print 
according to this grid.

  The Engine class has a map that maps object ID and the raw pointer to the object.

  The Engine class also has a vector of unique pointers to objects, those pointers own the objects.
  
  One valid, alive object will be registered on both the map and the vector.
  
  Each Object records its own necessary information such as hp, height and it also has a vector of Movements.
  
  Movement is a superclass which has 4 different subclasses which are player controlled movement, straight line movement, gravity movement, cycle movement.



#Design

In order to update the game border every 0.05 second, a Timer class is created and owned by the Engine. Timer has the function of startTime, so when the Engine 
first time calls function go and start the game, it will also call the timer to start time. StartTime works by saving the current time and calculating the difference
between new current time and old current time in an infinite loop in the scale of milliseconds, if the difference is greater or equal to 50 milliseconds, notify 
observers and update the old current time to newer current time so that the timer will calculate for the next 0.05 second. In this way, it is guaranteed that the game 
will be refreshed every 0.05 seconds which makes the game pretty smooth.

To have the information of what need to be printed on the screen, the Engine has a grid of 22*80 cells. Each cell contains the information of row number, 
column number and a vector of pointers to the object that lays on that cell. Everytime Engine makes a change to the grid and wants to refresh a cell on the screen, 
Engine can simply go through the vector of pointers on that cell and find the one with highest height, and then Engine will nail down to that object. Since every 
object has a vector of triple which records the row numbers, column numbers and displayed char, Engine will go through the vector of triples and find the triple whose 
row and column number matches with the cell that I want to refresh. Next we can get the char from that triple and print it on the screen.

Engine has the job to create objects. While creating objects, Engine will create a unique pointer to own that object and assign the vector of triples to the 
object and assign a vector of movement to the object. Engine will store the unique pointer to an object list which contains all the unique pointers to objects. Engine 
will have a bunch of raw pointers from unique pointers, Engine will store raw pointers with a unique id number in a map and it also stores the raw pointers in the 
cell. 

A map that maps object id with raw pointer is needed because after Timer calls timeNotify, Engine wants to traverse all of the objects and deal with their 
movements. In order to traverse all of the objects, a vector of objects is not wanted because if Engine deletes or adds some of the objects due to some collision while 
dealing with one object, the vector will shrink or expand, the iterator will point to the wrong location. A map is necessary because Engine records the highest id 
number and Engine could just traverse from id 0 to the highest id number. If some object is deleted, the map will not find that id number which will not cause any 
problem. 

To deal with movement, the Engine will traverse the map. For each object, the Engine will let the object do their abilities like generate other objects or 
change its movement. After finalising the object state, Engine will call nextPos method from the object and get the new position that the object wants to move to. For 
nextPos methods, an object traverses through its vector of movements and calls ifmove from each movement and if there’s no movement, it will return the vector triple 
of the current position. Every moment has its progress and speed, progress start with 0 and increase by 1 every time ifmove is called. If progress equals speed, it 
means it’s time to move and ifmove will return true, otherwise it will return false. Object is notified that it’s time to move, it will pass its current position to 
the movement and each type of movement will have its own method to calculate and return the new position.

After the Engine gets this new position, it will call collision detection to check collision on the new position. Client needs to define what will happen if 
two types of objects with same height collide. If some objects need to be deleted, it will add to a vector called needDeleted. Objects from the needDeleted vector will 
be deleted on the map and grid after the current object’s movement is finished.

If the object is not blocked and permitted to move to the new position, Engine will first clear the old position by removing the pointer to that object from 
the vector for the old position cells and update those cells to print what should be printed after the object is removed from the old cells. And the Engine will also 
update the new position and register the object onto those cells. And at the end change the position(vector of triples) for the object to the new position. 

In order to start the game, Engine has a method called go and it will lead to the level that the player wants and go will call start time if start time is 
first time being called, so start time is called once per game because Engine only needs to be notified once every 0.05 second. If a player is permitted to enter the 
next level, Engine will call go with the next level  at the beginning when Engine is time notified. Every time the Engine goes to the next level, it will clear 
everything on the board. Engine will clear the unique pointers and all those raw pointers from the vectors/maps. And the Engine will create new objects for the new 
level.

Clients could also update the view with colours, colour pairs need to be defined and the view could add char with specific colour on the screen.
	
In order for an object to move towards another, the target object pointer needs to be recorded. Before the Engine tries to move the follower objects, it will 
firstly change the direction of its movement according to the difference between the position of the follower object and the target object. And then call the follower 
object’s movement to get the new position.
