import pandas as pd 
import plotly.express as px 
file = 'data.dat'
df = pd.read_csv(file, sep=' ', header=None)
dict_cols = {}
no_cols = len(df.columns)
dict_cols[0] = 'time'
dict_mols = []
for c in range(1, no_cols):
    dict_cols[c] = f"x_{c}"
    dict_mols.append(f"x_{c}")
df = df.rename(columns=dict_cols)
fig = px.line(df, x='time', y= dict_mols)
fig.show()
