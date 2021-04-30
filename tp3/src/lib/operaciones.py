import sys
import math
import random
import heapq
from collections import defaultdict
from lib.grafo.grafo import Grafo
from lib.cola.cola import Cola
from lib.pila.pila import Pila

SEPARADOR = f"{chr(9)}"
sys.setrecursionlimit(50000)

# Esta variable global se utilizará para calcular las componentes fuertemente conexas,
# irá actualizando el orden de cada vertice a medida que el algoritmo recorre el grafo.

INDEX = 0

#########################################################################################
                                                                                        #
                        # LIBRERIA DE OPERACIONES SOBRE GRAFOS                          #
                                                                                        #
######################################################################################### 

def cargar_contenido(ruta_archivo):
    """Lee el contenido de _ruta_archivo_ y devuelve un grafo
    dirigido donde cada vertice es una página y cada arista es
    el enlace de una v a otra.
    """
    red_internet = Grafo(True)
    with open(ruta_archivo) as archivo:
        for linea in archivo:
            linea = linea.rstrip("\n").split(SEPARADOR)
            
            if linea[0] == "": continue
            pagina = linea[0]

            if not red_internet.pertenecen(pagina): red_internet.agregar_vertice(pagina)
            
            for i in range(1, len(linea)):
                if not red_internet.pertenecen(linea[i]): red_internet.agregar_vertice(linea[i])
                red_internet.agregar_arista_dirigido(pagina, linea[i])

    return red_internet


def camino_minimo_bfs(grafo, origen, destino=None):
    """Recibe un grafo, origen, y destino (opcional)
    Calcula el camino mínimo desde _origen_ hasta destino, en caso de no hacer uso
    del parámetro destino, calcula todos los caminos minimos desde cada vertice a _origen_.
    Si _destino_ no existe dentro del grafo devuelve una tupla con None, en caso de existir
    camino/s minimo/s devuelve una tupla de padres y distancias.
    Pre: el grafo fue creado y origen pertenece al grafo.
    """ 
    q = Cola()
    visitados = set()
    distancia = {}
    padres = {}

    q.encolar(origen)
    visitados.add(origen)
    distancia[origen] = 0
    padres[origen] = None
    
    while not q.esta_vacia():
        v = q.desencolar()
        
        if v == destino: return padres, distancia

        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                distancia[w] = distancia[v] + 1
                padres[w] = v
                visitados.add(w)
                q.encolar(w)
    
    if not destino: return padres, distancia
    return None, None


def agregar_componente(componentes, v, pila):
    """Recibe una pila, un vertice y una lista de componentes.
    Agrega a la lista una nueva lista con una nueva componente que se obtiene
    desapilando cada elemento de la pila hasta encontrarse con _v_.
    """
    componente = list()
    
    while True:
        w = pila.desapilar()
        componente.append(w)
        if w == v: break

    for vertice in componente:
        componentes[vertice] = componente

def _cfc(grafo, v, visitados, mb, orden, pila, componentes):
    """Recorre _grafo_ con el recorrido dfs y calcula las componentes fuertemente conexas."""

    visitados.add(v)
    pila.apilar(v)
    mb[v] = orden[v]
    
    for w in grafo.obtener_adyacentes(v):
        if w not in visitados:
            global INDEX
            orden[w] = INDEX + 1
            INDEX += 1
            _cfc(grafo, w, visitados, mb, orden, pila, componentes)
        
        if w in pila: mb[v] = min(mb[v], mb[w])

    if mb[v] == orden[v] and not pila.esta_vacia(): agregar_componente(componentes, v, pila)

def cfc(grafo):
    """Recibe un grafo y le calcula las componentes fuertemente conexas devolviendo una lista
    con cada una de las componentes.
    Pre: el grafo fue creado.
    """
    visitados = set()
    mb = dict()
    orden = dict()
    pila = Pila()
    componentes = dict()

    for v in grafo:
        if v not in visitados:
            orden[v] = 0
            _cfc(grafo, v, visitados, mb, orden, pila, componentes)

    return componentes


def heap_a_lista(q):
    """Quita los elementos de un heap de minimos y devuelve una lista con los elementos
    ordenados de mayor a menor.
    """
    solucion = list()
    
    while len(q) > 0:
        pr, pagina = heapq.heappop(q)
        solucion.insert(0, pagina)

    return solucion

def calcular_grados_y_padres(grafo):
    """Calcula los grados de entrada y padres de cada vertice de _grafo_ devolviendo
    dos diccionarios con la información.
    """
    grados_entrada = defaultdict(lambda : 0)
    padres = defaultdict(lambda : [])
    visitados = set()
    
    for v in grafo:
        if v not in visitados:
            visitados.add(v)
            padres[v] = None
            grados_entrada[v] = 0

        for w in grafo.obtener_adyacentes(v):
            visitados.add(w)
            if not padres[w]: padres[w] = list()
            padres[w].append(v)
            grados_entrada[w] += 1
    
    return grados_entrada, padres

def calcular_page_rank(grafo, padres, dic_pr, q, grados, d, n, k):
    """Itera _k_ veces _grafo_, calcula el page rank de cada vertice y agrega el resultado
    de la ultima iteración a un heap.
    """
    for i in range(1, k):
        for v in grafo:
            suma = 0
            if not padres[v]: suma = dic_pr[v][i - 1]
            else:
                for padre in padres[v]:   
                    if grados[padre] == 0: continue
                    suma += dic_pr[padre][i - 1] / len(grafo.obtener_adyacentes(padre))
            
            dic_pr[v][i] = ((1 - d) / n) + d * suma
            
    for v in grafo: heapq.heappush(q, (dic_pr[v][k - 1], v))
    
def page_rank(grafo, k=15):
    """Calcula el page rank de cada vertice del grafo iterandolo _k_ veces y devuelve una
    lista ordenada de mayor a menor valor de page rank.
    Pre: el grafo fue creado.
    """
    n = len(grafo)
    q = list()
    dic_pr = dict()
    
    grados, padres = calcular_grados_y_padres(grafo)
    for v in grafo: dic_pr[v] = {0: 1 / n}

    calcular_page_rank(grafo, padres, dic_pr, q, grados, 0.85, n, k)

    return heap_a_lista(q)
    

def buscar_ciclos(grafo, origen, n, v_act, n_act, padres, visitados):
    """Algoritmo basado en backtraking que busca un ciclo de largo _n_ dentro de _grafo_.
    Devuelve True si lo encuentra, de lo contrario False. La solucion se encuentra en _padres_.
    """
    if n_act == n and n_act > 0: return v_act == origen
    if v_act == origen and n_act > 0: return False
    
    for w in grafo.obtener_adyacentes(v_act):
        if (w in visitados and w != origen) or w == padres[v_act]: continue
    
        padres[w] = v_act
        visitados.add(w)

        hay_solucion = buscar_ciclos(grafo, origen, n, w, n_act + 1, padres, visitados)
        
        ultimo = visitados.remove(w)

        if hay_solucion: return True

    return False

def obtener_ciclo_n(grafo, origen, n):
    """Recibe un grafo y busca un ciclo de largo _n_ dentro de la red utilizando backtracking,
    si encuentra uno devuelve True y un diccionario de padres con el ciclo. De lo contrario, 
    False y un diccionario vacio.
    Pre: el grafo fue creado.
    """
    visitados = set()
    padres = dict()
    n_act = 0
    
    padres[origen] = None
    visitados.add(origen)

    return buscar_ciclos(grafo, origen, n, origen, n_act, padres, visitados), padres


def obtener_diametro(grafo):
    """Devuelve el diametro del grafo, el vértice que se encuentra a esa distancia y un diccionario
    para poder reconstruir el camino. Recibe como parametro el grafo.
    Pre: el grafo fue creado.
    """
    max_dist_min = 0
    destino = None
    camino = None

    for v in grafo:
        padres, distancia = camino_minimo_bfs(grafo, v)
        for w in distancia:
            if distancia[w] > max_dist_min:
                max_dist_min = distancia[w]
                destino = w
                camino = padres

    return max_dist_min, destino, camino


def rango_n(grafo, vertice, n):
    """Devuelve la cantidad de vértices que se encuentra a distancia n de un vértice. Recibe como
    parámetro el grafo, un vértice y un número n.
    Pre: el grafo fue creado y el vértice se encuentra en él.
    """
    distancia = camino_minimo_bfs(grafo, vertice)[1]
    contador = 0

    for w in distancia:
        if distancia[w] == n: contador += 1
    
    return contador


def _camino_dfs(grafo, v, contador, camino):
    if contador > 20: return
    
    camino.append(v)
    adyacentes = grafo.obtener_adyacentes(v)
    
    if len(adyacentes) == 0: return
    
    _camino_dfs(grafo, adyacentes[0], contador + 1, camino)

def camino_dfs(grafo, origen):
    """Devuelve un camino desde un vértice hasta otro, sólo accediendo a la primer arista de los vértices.
    El camino termina cuando el último vértice no tiene adyacentes o cuando se recorriendo 20 vértices.
    Recibe como parámetro el grafo y un origen.
    Pre: el grafo fue creado y el origen pertenece a él.
    """
    contador = 0
    camino = []
    
    _camino_dfs(grafo, origen, contador, camino)
    
    return camino


def obtener_promedio_clustering(grafo):
    """Devuelve el promedio de los coeficientes Clustering de todos los vértices del grafo.
    Pre: el grafo fue creado.
    """
    sumatoria = 0
    n = len(grafo)

    for v in grafo: sumatoria += obtener_coef_clustering(grafo, v)

    return sumatoria / n

def obtener_coef_clustering(grafo, vertice):
    """Devuelve el coeficiente Clustering de un vértice. Recibe como parámetros el grafo y el 
    vértice en cuestión.
    Pre: el grafo fue creado y el vértice pertenece a él.
    """
    cant = 0
    adyacentes = grafo.obtener_adyacentes(vertice)
    grado_salida = len(adyacentes)
    
    if grado_salida < 2: return 0
    
    for v in adyacentes:
        if v == vertice: continue
        for w in adyacentes:
            if w == v: continue
            if grafo.estan_conectados(v, w): cant += 1
            
    return cant / (grado_salida * (grado_salida - 1))

def calcular_clustering(grafo, origen):
    if not origen: return obtener_promedio_clustering(grafo)
    else: return obtener_coef_clustering(grafo, origen)

def obtener_arista_entrada_todos(grafo):
    """Devuelve un diccionario con la lista de vértices de entrada de cada vértice del grafo.
    Recibe como parametro un grafo ya creado.
    """
    entrada = {}

    for v in grafo: entrada[v] = []

    for v in grafo:
        for w in grafo.obtener_adyacentes(v): entrada[w].append(v)
    
    return entrada

def _orden_aleatorio_(grafo, v, visitados, orden):
    visitados.add(v)
    orden.append(v)
    
    for w in grafo.obtener_adyacentes(v):
        if w not in visitados: _orden_aleatorio_(grafo, w, visitados, orden)

def orden_aleatorio(grafo):
    """Devuelve un orden aleatorio en el cual se puede recorrer el grafo (recorrido dfs)."""

    visitados = set()
    orden = []

    vertices = grafo.obtener_vertices()
    i = random.randrange(0, len(vertices))
            
    _orden_aleatorio_(grafo, vertices[i], visitados, orden)

    for v in grafo:
        if v not in visitados: _orden_aleatorio_(grafo, v, visitados, orden)
    
    return orden

def max_frecuencia(comunidades, v, aristas_entrada):
    """Devuelve la comunidad con mayor frecuencia en las aristas de entrada de un vértice. 
    Recibe como parámetro un dic con todos las comunidades del grafo, el vértice en cuestión y 
    las aristas de entrada del mismo.
    """
    frecuencia = {}
    maximo = 0
    label = comunidades[v]

    for w in aristas_entrada:
        frecuencia[comunidades[w]] = frecuencia.get(comunidades[w], 0) + 1
    
    for f in frecuencia:
        if frecuencia[f] >= maximo:
            label = f
            maximo = frecuencia[f]
    
    return label

def label_propagation(grafo, iteraciones=100):
    """Algoritmo para calcular las comunidades que existen en un grafo. Recibe como parametro 
    el grafo.
    """
    label  = {}
    entrada = obtener_arista_entrada_todos(grafo)

    for v in grafo: label[v] = v

    for i in range(iteraciones):
        orden = orden_aleatorio(grafo)
        for v in orden: label[v] = max_frecuencia(label, v, entrada[v])
    
    return label