import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')  # Use the Agg backend for rendering to files
import pandas as pd

# Read the data from the results.csv file
df = pd.read_csv('results.csv', sep=';')

# Prepare the data by combining input width and height into a single resolution column
df['input_resolution'] = df['input_width'].astype(str) + 'x' + df['input_height'].astype(str)

# Group the data by input resolution and output width, calculating the average time for each combination
df_grouped = df.groupby(['input_resolution', 'output_width']).agg({'time': 'mean'}).reset_index()

# Create a plot
plt.figure(figsize=(10, 6))

# Iterate over each unique input resolution to plot the data
for resolution in df_grouped['input_resolution'].unique():
    data_res = df_grouped[df_grouped['input_resolution'] == resolution]
    plt.plot(data_res['output_width'], data_res['time'], label=resolution, marker='o')

# Adding labels and title
plt.xlabel('Output Width')
plt.ylabel('Time (ms)')
plt.title('Resize Time vs Output Width for Different Input Resolutions')

# Show legend
plt.legend(title='Input Resolution')

# Display the plot
plt.grid(True)
plt.tight_layout()
plt.savefig('resize_time_plot.png', dpi=300)
plt.close()
