
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

### **Passo a Passo**

1. **Clonar o Repositório**
   - Acesse o repositório em:  
     https://github.com/mariajuliadantas/JOGO-PIF.git  
   - Copie o link HTTPS e clone o repositório em sua máquina com o comando:
     `git clone https://github.com/mariajuliadantas/JOGO-PIF.git`
   - Entre na pasta do projeto com:
     `cd JOGO-PIF`

2. **Verificar e Instalar Requisitos**
   - Certifique-se de que `gcc` e `make` estão instalados no sistema.
   - Para verificar, use os comandos:
     `gcc --version` e `make --version`.
   - Caso não estejam instalados:
     - **Linux:** Instale usando `sudo apt update && sudo apt install build-essential -y`.
     - **macOS:** Instale usando `brew install gcc make`.
     - **Windows:** Baixe o MinGW e adicione o `gcc` e `make` ao PATH do sistema.

3. **Compilar o Jogo**
   - Dentro da pasta do projeto, use o comando:
     `make all`
   - Este comando compilará o código e gerará o executável do jogo.

4. **Executar o Jogo**
   - Após compilar, inicie o jogo com o comando:
     `make run`
   - O jogo será iniciado no terminal, com as instruções exibidas na tela.
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
