install.packages("readxl")
install.packages("ggplot2")
install.packages("data.table")
library(data.table) 
require(ggplot2)
library(readxl)
library(ggplot2)

df1 <- read_excel("C:/Users/pedro/Desktop/PrjPE/econ.xlsx", sheet = 1)

#retirar datas antes de 1971
dados <- df1[-(1:54),]

dados2 <- dados[,c("tempo","pop","tpp")]
media_dd <- mean(dados2$pop)
dp_dd <- sd(dados2$pop)
media_tpp <- mean(dados2$tpp)
dp_tpp <-sd(dados2$tpp)

#transformacao de dados
dados2$pop <- (dados2$pop - media_dd) / dp_dd
dados2$tpp <- (dados2$tpp - media_tpp) / dp_tpp


ggplot(dados2, aes(dados2$tempo)) +
  geom_line(aes(y = pop, color = "pop"), size = 1) +
  geom_line(aes(y = tpp, color = "tpp"), size = 1) +
  labs(title="Variação de pop e tpp a partir de 1971" ,x = "Ano", y = "Valores Corrigidos", color = "Variável") +
  scale_color_manual(values = c(pop = "blue", tpp = "red")) +
  theme_minimal()
