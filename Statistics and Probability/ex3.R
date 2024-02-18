install.packages("ggplot2")
install.packages("data.table")
library(data.table) 
require(ggplot2)
library(ggplot2)

df1 <- read.csv("C:/Users/pedro/Desktop/PrjPE/GENDER_EMP_19032023152556091.txt",header=T, sep="\t")

franca <- df1[df1$COU=='FRA' & df1$IND=='EMP3' & df1$TIME=="2018" & df1$AGE!='TOTAL',]

dt <- franca[franca$AGE!="1564" & franca$SEX!='ALL_PERSONS',]


ggplot(dt, aes(Age.Group, Value, fill = SEX)) + 
  labs(title = "Unemployment/population ratio by Sex and Age Group in France 2018",
       x = "Age Group",
       y = "Unemployment Value")+
  geom_col(position = "dodge2")
