# Trabalho de compiladores 1

## Integrante
Wellington Albertoni Miranda de Campos (2019.1905.048-5)

## Compilação
Compilado usando `make build` 

```bash
$ g++ --version
g++ (Debian 12.2.0-14+deb12u1) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
```

## Observação

Realizei uma alteração na produção 38 de

```
38. UnaryExpression → + Factor

                    | - Factor
```
Para

```
38. UnaryExpression → + Factor

                    | - Factor

                    |   Factor
```
Respeitando a produção do livro base.