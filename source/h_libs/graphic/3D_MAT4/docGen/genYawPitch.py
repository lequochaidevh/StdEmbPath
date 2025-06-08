import numpy as np
import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

# Góc phi và psi (đơn vị rad)
phi = np.pi / 4      # 45 độ
psi = np.pi / 3      # 60 độ
r = 1

# Tọa độ vector r theo hệ cầu
x = r * np.sin(phi) * np.cos(psi)
y = r * np.sin(phi) * np.sin(psi)
z = r * np.cos(phi)

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111, projection='3d')

# Vẽ trục
ax.quiver(0,0,0,1,0,0,color='r', arrow_length_ratio=0.1)
ax.text(1.1,0,0,'X', color='r', fontsize=14)
ax.quiver(0,0,0,0,1,0,color='g', arrow_length_ratio=0.1)
ax.text(0,1.1,0,'Y', color='g', fontsize=14)
ax.quiver(0,0,0,0,0,1,color='b', arrow_length_ratio=0.1)
ax.text(0,0,1.1,'Z', color='b', fontsize=14)

# Vẽ vector r
ax.quiver(0,0,0,x,y,z,color='k', linewidth=2, arrow_length_ratio=0.1)
ax.text(x,y,z,'P', fontsize=14)

# Vẽ góc phi (dùng cung tròn trong mặt phẳng Z-r)
phi_vals = np.linspace(0, phi, 100)
phi_x = r * np.sin(phi_vals)
phi_z = r * np.cos(phi_vals)
ax.plot(phi_x, np.zeros_like(phi_vals), phi_z, 'b--', label='phi')

# Vẽ góc psi (cung tròn trong mặt phẳng XY)
psi_vals = np.linspace(0, psi, 100)
psi_x = r * np.cos(psi_vals)
psi_y = r * np.sin(psi_vals)
ax.plot(psi_x, psi_y, np.zeros_like(psi_vals), 'r--', label='psi')

ax.set_xlim(0,1.2)
ax.set_ylim(0,1.2)
ax.set_zlim(0,1.2)

ax.legend()
plt.title("Spherical Coordinates: phi (blue), psi (red)")
plt.show()
