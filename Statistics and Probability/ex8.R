set.seed(1501)

n <- 178

# Amostra da distribuição de Cauchy
cauchy_sample <- rcauchy(n, location = -1.6, scale = 1)
ordered_cauchy <- sort(cauchy_sample)

quantiles_cauchy <- rep(0, n)
for (i in 1:n) {
  quantiles_cauchy[i] <- i / (n + 1)
}

# Amostra da distribuição normal
mu <- 3.6
sigma <- sqrt(3.4)
normal_sample <- rnorm(n, mean = mu, sd = sigma)
ordered_normal <- sort(normal_sample)

quantiles_normal <- rep(0, n)
for (i in 1:n) {
  quantiles_normal[i] <- i / (n + 1)
}

# Gráfico
plot(quantiles_cauchy, ordered_cauchy, type = "p", col = "blue",
     xlab = "Quantis de Probabilidade", ylab = "Valores Gerados",
     main = "Distribuição de Cauchy vs Distribuição Normal",
     ylim=c(-50,65))

lines(quantiles_normal, ordered_normal, type = "p", col = "red")

abline(a = 0, b = 1, lty = 1)

legend("topleft", legend = c("Cauchy", "Normal"), col = c("blue", "red"), lty = 1,)
