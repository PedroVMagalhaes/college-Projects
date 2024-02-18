set.seed(1817)
n<-1170
p<-0.5

# Função de distribuição acumulada da distribuição geométrica
distribuicao <- function(x, p) {
  if (x < 0) {
    return(0)
  } else {
    return(1 - (1 - p)^(x + 1))
  }
}

# Gerar amostra de valores da distribuição geométrica
amostra <- numeric(n)
for (i in 1:n) {
  u <- runif(1)  # Simular valor de uma distribuição uniforme
  x <- 0        # Valor inicial de x
  
  # Encontrar o valor de x usando o método de transformação inversa
  while (distribuicao(x, p) < u) {
    x <- x + 1
  }
  
  amostra[i] <- x
}

# Calcular a média e o desvio padrão da amostra
media_amostral <- mean(amostra)
dp_amostral <- sd(amostra)

# Calcular a soma da média com o desvio padrão amostrais
soma_media_desvio <- media_amostral + dp_amostral

# Contadores para valores simulados que são superiores à média + desvio padrão amostrais e superiores apenas à media
superiores_media_desvio <- 0
superiores_media <- 0

# Verificar a proporção de valores que são superiores à média + desvio padrão amostrais
for (i in 1:n) {
  if (amostra[i] > media_amostral) {
    superiores_media <- superiores_media + 1
  }
  if (amostra[i] > (media_amostral + dp_amostral)) {
    superiores_media_desvio <- superiores_media_desvio + 1
  }
}

# Calcular a proporção de valores simulados que são superiores à média + desvio padrão amostrais
proporcao <- superiores_media_desvio / superiores_media

# Arredondar para 4 casas decimais
proporcao_arredondada <- round(proporcao, 4)

proporcao_arredondada
