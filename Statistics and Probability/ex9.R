library(ggplot2)
require(ggplot2)

set.seed(1579)
k <- 1000
p <- 0.3
nivel_c <- 0.93
gamma <- 1 - nivel_c
b<-qnorm(1-gamma/2)  #-b__normal__a a=-b

#como IC pelo metodo2  é entre xbarra - b(raiz de cenas) e xbarra+b(raiz de cenas)
#o comprimento do intervalo pelo metodo2 é igual a 2b(raiz de cenas)

n_values <- c(30, 50, 100, 200, 300, 500, 1000)
result_list <- list()
final <- vector("list", length(n_values))
mean_list <- numeric() #lista dos xbarra
average_difference<-numeric()  #lista das médias das diferenças entre metodo 1 e 2
z <- qnorm((1 + nivel_c) / 2)

for (i in 1:length(n_values)) {
  final[[i]] <- numeric(1000)  # inicializar sublista para cada valor de n
  
  for (j in 1:1000) {
    binom_values <- rbinom(n_values[i], 1, p)
    result_list[[j]] <- binom_values
    
    mean_list[j] <- mean(result_list[[j]])
    
    a <- n_values[i] + z^2
    b <- -2 * mean_list[j] * n_values[i] - z^2
    c <- n_values[i] * mean_list[j]^2
    
    delta <- b^2 - 4 * a * c
    
    if (delta >= 0) {
      p1 <- (-b + sqrt(delta)) / (2 * a)
      p2 <- (-b - sqrt(delta)) / (2 * a)
      
      comp_intervalo1<-abs(p1-p2) #comp_intervalo do metodo1
      
      #como IC pelo metodo2  é entre xbarra - b(raiz de cenas) e xbarra+b(raiz de cenas)
      #o comprimento do intervalo pelo metodo2 é igual a 2b(raiz de cenas)
      
      comp_intervalo2<- abs(2*b*sqrt((mean_list[j]-mean_list[j]^2)/n_values[i]))
      
      comp_intervalo<-abs(comp_intervalo1-comp_intervalo2)
      
      final[[i]][j]<-comp_intervalo
    }
  }
  average_difference[i]<-mean(final[[i]])
}

average_difference

df<- data.frame(n=n_values,average_diff=average_difference)

ggplot(df,aes(df$n))+
  geom_point(aes(y=average_diff,color="Diferença Média entre o Método 1 e Método 2"))+
  labs(title="Diferença Média dos Intervalos em função do Tamanho da amostra" ,x = "Tamanho da amostra", y = "Diferença média", color = "Variável")+
  theme_minimal()
