set.seed(561)  # Fixar a semente em 146

m <- 250  # Número de amostras
n <- 49  # Dimensão das amostras
m0<-59.5 #H0
mu <- 60.5  # Valor para usar na simulação
n_significancia<-0.1

# Simulação das amostras
samples_list<-list(numeric(n))
# Teste de hipóteses para cada amostra
#test_results <- numeric(m)

p_value<-numeric(m)

for (i in 1:m) {
  samples_list[[i]] <- rnorm(n, mean = mu, sd = 4)
  t_stat <- (mean(samples_list[[i]]) - 59.5) / (2 / sqrt(n))  # Estatística t
  p_value[i]<-2*(1-pnorm(abs(t_stat)))
}

# Estimativa da probabilidade de não rejeição de H0
probability <- sum(p_value>=n_significancia)/m

probability_rounded<-round(probability,digits = 3)

probability_rounded
