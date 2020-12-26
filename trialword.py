import wordcloud as wc
c  = wc.WordCloud()
c.generate("hey this is the first word I want to say and thank you for the help")
c.to_file("ima.png")
print("sdf")