
# 🗺️ Caça ao Tesouro

Bem-vindo ao **Caça ao Tesouro**, um jogo de aventura interativo no console! Explore mapas, colete chaves, evite inimigos e descubra o tão desejado tesouro. 🏆

---

## 📖 Sobre o Jogo

No **Caça ao Tesouro**, você é um explorador corajoso em busca de um tesouro perdidodido. Mas cuidado! O caminho está cheio de perigos, inimigos móveis e desafios que exigem estratégia e conhecimento.

### **Destaques do Jogo**
- **Pistas escondidas:** Marcadas com "P", elas desafiam você com perguntas.
- **Inimigos móveis:** Eles adicionam dificuldade e precisam ser evitados.
- **Tesouro trancado:** Recolha 3 chaves para abrir o baú.
- **Registro de tempo:** Veja quanto tempo levou para encontrar o tesouro.

---

## 🎮 Como Jogar

- **Movimentação:** Use as teclas `W`, `A`, `S`, `D` ou as setas direcionais.
- **Objetivo:** Encontre o baú do tesouro (marcado como "T") e colete 3 chaves para abrir.
- **Cuidado com os inimigos:** Colidir com eles ou errar respostas custa uma vida.
- **Dica:** Responda corretamente às perguntas das pistas para ganhar chaves.

---

## 🛠️ Instalação e Execução

### **Requisitos**
- **Sistema operacional:** Windows, Linux ou macOS.
- **Ferramentas:** `gcc` e `make`.

### **Passo a Passo**
1. Certifique-se de que `gcc` e `make` estão instalados:
   - **Linux/macOS:** Use o gerenciador de pacotes da sua distribuição.
   - **Windows:** Instale o MinGW e adicione ao PATH do sistema.

2. Clone o repositório do jogo e extraia-o para uma pasta no seu computador.

3. No terminal, navegue até a pasta onde o jogo foi salvo.

4. Compile o jogo:
   make all
   Isso irá compilar o código e gerar o executável do jogo.

5. Execute o jogo:
   make run

6. **Aproveite!** O jogo será iniciado no terminal. Siga as instruções exibidas na tela e divirta-se!

---

## 🧩 Detalhes do Código

### **Principais Funções**
- **`exibirHistoria`:** Apresenta uma introdução ao jogo.
- **`exibirInstrucoes`:** Mostra as regras e os controles.
- **`drawmap`:** Renderiza o mapa com todos os elementos do jogo.
- **`movePlayer`:** Atualiza a posição do jogador.
- **`moveEnemy`:** Define a movimentação dos inimigos móveis.
- **`checkCollision`:** Verifica colisões entre jogador e inimigos.

### **Destaques do Código**
- **Inimigos móveis:** Comportamento dinâmico que respeita barreiras no mapa.
- **Sincronização:** Atualização contínua para garantir fluidez no console.
- **Registro de tempos:** Armazena o desempenho dos jogadores no arquivo `tempos_tesouro.txt`.

---

## 🚧 Desafios Encontrados

1. **Movimentação dos Inimigos:**
   - **Problema:** Criar um comportamento realista para respeitar os limites do mapa.
   - **Solução:** Lógica de inversão de direção ao encontrar barreiras.

2. **Captura de Entrada em Tempo Real:**
   - **Problema:** Permitir a movimentação do jogador sem bloquear o jogo.
   - **Solução:** Uso de `keyboard.h` para capturar teclas de forma assíncrona.

3. **Sincronização de Elementos:**
   - **Problema:** Atualizar jogador, inimigos e perguntas em tempo real.
   - **Solução:** Implementação de ciclos curtos com `usleep` para garantir fluidez.

---

## 🗂️ Equipe
- Henrique Gueiros
- Lucas Calabria 
- Maria Júlia Dantas
