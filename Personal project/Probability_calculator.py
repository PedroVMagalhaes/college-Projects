
import math

def calculate_probability_recursive(wins, losses, target_wins, max_losses, win_prob, loss_prob):
    # Stop wins
    if wins == target_wins:
        return 1.0
    
    # Stop Loses
    if losses == max_losses:
        return 0.0
    
    # % of wining
    win_probability = win_prob * calculate_probability_recursive(wins + 1, losses, target_wins, max_losses, win_prob, loss_prob)
    loss_probability = loss_prob * calculate_probability_recursive(wins, losses + 1, target_wins, max_losses, win_prob, loss_prob)
    
    return win_probability + loss_probability

def calculate_probabilities_up_to_with_losses(target_wins, max_losses, win_prob, loss_prob):
    probabilities = {}
    for i in range(target_wins + 1):
        probability = calculate_probability_recursive(0, 0, i, max_losses, win_prob, loss_prob)
        probabilities[i] = probability
    
    return probabilities

def calculate_binomial_probability(n, k, p):
    """
   Binomial?? @anti
    """
    return math.comb(n, k) * (p ** k) * ((1 - p) ** (n - k))

def calculate_probabilities_up_to_without_losses(target_wins, win_prob):
    probabilities = {}
    for i in range(target_wins + 1):
        probability = calculate_binomial_probability(target_wins, i, win_prob)
        probabilities[i] = probability
    
    return probabilities

#Using it:
target_wins = int(input("Número de vitórias: "))
max_losses_input = input("Número máximo de derrotas(numero, ou deixar em branco se for sem derrotas): ")

# Check if max_losses_input is empty
if max_losses_input.strip() == "":
    max_losses = None
else:
    max_losses = int(max_losses_input)

win_prob = float(input("Probabilidade de ganhar (0.0 a 1.0): "))
loss_prob = 1 - win_prob

if ((0.0 <= win_prob <= 1.0)):
    if max_losses is not None:
        probabilities = calculate_probabilities_up_to_with_losses(target_wins, max_losses, win_prob, loss_prob)
    else:
        probabilities = calculate_probabilities_up_to_without_losses(target_wins, win_prob)
    
    for wins, probability in probabilities.items():
        print(f"A probabilidade de alcançar {wins} vitórias é: {probability:.4f}")
