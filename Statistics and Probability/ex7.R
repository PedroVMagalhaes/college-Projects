set.seed(1764)
m<-2549
n<-16

samples <- matrix(rnorm(m * n), nrow = m)

sum_of_squares <- rowSums(samples^2)

quantile_sample <- quantile(sum_of_squares, probs = 0.32, type = 2)

quantile_theoretical <- qchisq(0.32, df = n)

difference <- abs(quantile_sample - quantile_theoretical)
difference <- round(difference, 4)

difference
