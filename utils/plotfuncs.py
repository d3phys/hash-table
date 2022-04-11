import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

data  = pd.read_csv('out', index_col = 0, sep=',').T
index = np.linspace(0, 255, 256)

fig, axs = plt.subplots(3, 2)

axs[0][0].bar(index, data['first_ascii_hash'],       color ='brown')
axs[0][1].bar(index, data[        'ror_hash'],       color ='green')
axs[1][0].bar(index, data[  'sum_ascii_hash'],       color ='green')
axs[1][1].bar(index, data[     'length_hash'],       color ='brown')
axs[2][0].bar(index, data[      'crc32_hash'],       color ='green')
axs[2][1].bar(index, data[        'one_hash'],       color ='brown')

axs[0][0].set_title('first_ascii_hash');
axs[0][1].set_title(        'ror_hash');
axs[1][0].set_title(  'sum_ascii_hash');
axs[1][1].set_title(     'length_hash');
axs[2][0].set_title(      'crc32_hash');
axs[2][1].set_title(        'one_hash');

plt.gcf().set_size_inches(10, 10)

plt.subplots_adjust(
    left   =0.1,
    bottom =0.1, 
    right  =0.9, 
    top    =0.9, 
    wspace =0.4, 
    hspace =0.4
)

plt.savefig('./img/plot.png')
