# these runs are collected frmo Level 1, setting all attributes to 3 and the orientation marker at the right side of the island.
library(ggplot2)
library(gridExtra)

myData0 <- read.csv('lvl1_run0.csv', header=T, sep=";")
myData0$run = 0
myData1 <- read.csv('lvl1_run1.csv', header=T, sep=";")
myData1$run = 1
myData2 <- read.csv('lvl1_run2.csv', header=T, sep=";")
myData2$run = 2
myData3 <- read.csv('lvl1_run3.csv', header=T, sep=";")
myData3$run = 3
myData4 <- read.csv('lvl1_run4.csv', header=T, sep=";")
myData4$run = 4
myData5 <- read.csv('lvl1_run5.csv', header=T, sep=";")
myData5$run = 5

myData <- rbind(myData0, myData1)
myData <- rbind(myData, myData2)    
myData <- rbind(myData, myData3)
myData <- rbind(myData, myData4)
myData <- rbind(myData, myData5)

g1 <- ggplot(myData, aes(x=step, y=pop, col=factor(run))) + geom_line() + geom_point() + ggtitle('pop vs time')
g2 <- ggplot(myData, aes(x=step, y=time, col=factor(run))) + geom_line() + geom_point() + ggtitle('cpu vs time')
g3 <- ggplot(myData, aes(x=step, y=time/pop, col=factor(run))) + geom_line() + geom_point() + ggtitle('cpu/pop vs time')
g4 <- ggplot(myData, aes(x=pop, y=time, col=factor(run))) + geom_point() + ggtitle('pop vs cpu')

pdf('benchmark_lvl1.pdf', width=12)
grid.arrange(g1,g2,g3,g4, main="lvl 1")
dev.off()

