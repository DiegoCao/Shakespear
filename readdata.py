import os, sys, email,re
import numpy as np 
import pandas as pd
import community
# Plotting
import matplotlib.pyplot as plt

import seaborn as sns; sns.set_style('whitegrid')
import wordcloud

# Network analysis, used for create social Graph
import networkx as nx

# NLP: used for dealing with words in python

from nltk.tokenize.regexp import RegexpTokenizer

from subprocess import check_output

# ML: used for dealing with machine learning method
from sklearn.feature_extraction.text import TfidfVectorizer,CountVectorizer
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
#from sklearn.lda import LDA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.decomposition import LatentDirichletAllocation

import gensim
from gensim import corpora
from nltk.corpus import stopwords 
from nltk.stem.wordnet import WordNetLemmatizer
import string
from nltk.stem.porter import PorterStemmer


NROWS = 200

emails_df = pd.read_csv('emails.csv', nrows= NROWS)

print(emails_df.shape)
print(emails_df)



def get_text_from_email(msg):
    '''To get the content from email objects'''
    parts = []
    for part in msg.walk():
        if part.get_content_type() == 'text/plain':
            parts.append( part.get_payload() )
    return ''.join(parts)

def split_email_addresses(line):
    '''To separate multiple email addresses'''
    if line:
        # line = line.replace('@enron.com', '')
        addrs = line.split(',')
        # addrs = frozenset(map(lambda x: x.strip(), addrs))
        addrs = frozenset(map(lambda x: x.strip(), addrs))
    else:
        addrs = None
    return addrs

def removeExtra(line):
    if line:
        line = line.strip('@')
    return line


# Get all messages to strings
messages = list(map(email.message_from_string, emails_df['message']))
emails_df.drop('message', axis=1, inplace=True)
# Get fields from parsed email objects
print(emails_df)


keys = messages[0].keys()
print(keys)
for key in keys:
    emails_df[key] = [doc[key] for doc in messages]
    
    
# Parse content from emails

emails_df['content'] = list(map(get_text_from_email, messages))

# Split multiple email addresses
emails_df['From'] = emails_df['From'].map(split_email_addresses)
emails_df['To'] = emails_df['To'].map(split_email_addresses)

# Extract the root of 'file' as 'user'
emails_df['user'] = emails_df['file'].map(lambda x:x.split('/')[0])
del messages
emails_df.head()

new_email_df = pd.DataFrame(data = emails_df,columns=['From', 'To'])

# new_email_df['From'] = new_email_df['From'].map(removeExtra)
print(new_email_df)
df_2 = pd.DataFrame(data = emails_df, columns= ['X-From','X-To'])


df_2.columns = ['From', 'To']



G = nx.Graph()

#### deal with the Graph

for index, row in new_email_df.iterrows():
    if row['To']:
        for item in row['To']:
            if(G.has_edge(row['From'], item)):
                G[row['From']][item]['weight'] = G[row['From']][item]['weight'] + 1
            else:
                G.add_weighted_edges_from([(row['From'],item, 1)])



            
nx.draw(G,with_labels = True)
# nx.draw(G, with_labels = False)

# plt.show()
# plt.savefig("sample2.png")



partition = community.best_partition(G)
print(type(partition))
i = 0



# Sort it as the graph!

# first do commmunity detection, use the set graph(GNN/statistic)
# 

