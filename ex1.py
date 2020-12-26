from community import best_partition as cp 
import networkx as nx
G = nx.erdos_renyi_graph(100, 0.01)
partion = cp(G)
print(partion)