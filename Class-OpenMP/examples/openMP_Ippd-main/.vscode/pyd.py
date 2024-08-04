import cv2

# Cria uma imagem preta
image = cv2.imread('/home/frank/Imagens/Captura de tela de 2024-04-17 16-36-31.png')  # Substitua pelo caminho de uma imagem em seu sistema

# Mostra a imagem
cv2.imshow('Imagem', image)

# Espera até que uma tecla seja pressionada
cv2.waitKey(0)

# Fecha todas as janelas abertas
cv2.destroyAllWindows()
