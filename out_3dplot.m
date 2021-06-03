file = load('track.mat');
x = file.ans.x.Data(:,:);
y = file.ans.y.Data(:,:);
z = file.ans.z.Data(:,:);

plot3(x, y, z);

title('Track in 3-D Space');
grid();