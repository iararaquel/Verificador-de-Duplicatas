# Implementação de Tabela Hash Genérica em C/ Verificador de Duplicatas

Este código é a implementação de uma Tabela hash genérica em linguagem C,
desenvolvida com o objetivo de armazenar pares de chave-valor de forma eficiente, com
suporte para rehashing, verificação de duplicatas, e manipulação genérica de dados
(usando ponteiros void *). A estrutura foi projetada para funcionar com qualquer tipo de
chave ou valor, desde que funções auxiliares específicas sejam fornecidas pelo usuário
(hash, comparação, liberação e impressão).

# O problema central consiste em criar uma estrutura de tabela hash eficiente, reutilizável e segura, que possa:

● Inserir elementos com verificação de duplicatas;

● Redimensionar automaticamente a tabela conforme o fator de carga;

● Buscar e remover elementos;

● Armazenar qualquer tipo de dado genérico;

● Exibir estatísticas como número de colisões e fator de carga.
Essa abordagem é útil em contextos onde há a necessidade de desempenho e flexibilidade,
como em interpretadores, sistemas de cache ou indexação de dados.

# Para resolver o problema, foi criada uma tabela hash baseada em listas encadeadas, com as seguintes estratégias:

● Encadeamento separado para tratamento de colisões;

● Função de hash djb2 para strings (pode ser substituída);

● Rehash automático quando o fator de carga atinge 0,75;

● Uso de funções ponteiro (callbacks) para permitir personalização do
comportamento da tabela quanto ao tipo de dado.

# Conclusão
A implementação cumpre eficientemente os requisitos de uma tabela hash genérica. Entre
os destaques:

● Flexibilidade de uso com qualquer tipo de dado;

● Eficiência na inserção e busca;

● Detecção de duplicatas e rehashing automático;

● Controle de colisões e fator de carga.
A estrutura pode ser estendida para suportar mais funcionalidades, como iteração,
exportação/importação de dados, ou suporte a tipos específicos via macros.
