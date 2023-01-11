CXX = g++
CXXFLAGS = -std=c++14 -Werror -MMD -lncurses -g
EXEC = game1
OBJECTS = controller.o curseControl.o curseView.o engine.o main.o mCycle.o mGravity.o model.o mPC.o mStraight.o object.o timer.o timerSubject.o triple.o movement.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${CXXFLAGS}

-include ${DEPENDS}

.PHONY:	clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
