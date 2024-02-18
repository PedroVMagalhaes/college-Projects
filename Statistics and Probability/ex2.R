install.packages("ggplot2")
install.packages("data.table")
library(data.table) 
require(ggplot2)
library(ggplot2)

df1 <- read.csv("C:/Users/pedro/Desktop/PrjPE/TIME_USE_24092022.csv",header=T,sep=',')

#apagar dados áfrica do sul
teste <- df1[df1$País!='África do Sul',]

total <- teste[teste$Sexo=='Total',]
naoRemunerado <- total[total$Ocupação=='Trabalho não remunerado',]
naoRemunerado <- naoRemunerado$Tempo
lazer <- total[total$Ocupação=='Lazer',]
lazer <- lazer$Tempo

par(mar = c(5, 10, 8, 8) + 0.1)

boxplot(naoRemunerado,lazer,
        main = "Tempos médios diários de total em atividades distintas",
        at = c(1,2),
        names=c("T. Não Remunerado","Lazer"),
        las=2,
        col=c("red","orange"),
        border="black",
        horizontal=TRUE,
        notch=FALSE)
