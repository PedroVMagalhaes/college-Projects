set.seed(3126)
lambda<-16
k<-4474

distribuicao<- rexp(k,rate=lambda)
s<-cumsum(distribuicao)
t<-s[k]      #cumsum total
t<-ceiling(t)   #arredondar para cima

#fazer intervalos unitários entre 0 e s4474
subintervalos <- seq(0,t, by=1)

#contar quantos numeros ocorrem entre cada unidade
contagem<-table(cut(s, breaks=subintervalos, include.lowest = FALSE))
media<-mean(contagem)
media

desvio_abs<-abs(media-lambda)
desvio_abs
desvio_arredondado<- round(desvio_abs,4)
desvio_arredondado
