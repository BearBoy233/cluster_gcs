# BaiduMap API JS V2.0 

## 搜索 
- 不用, 直接通过 qt 设置中心点 GPS 位置

```javascript
    <script type="text/javascript">
     var local = new BMap.LocalSearch(map, {
		renderOptions:{map: map}
	 });
     local.search("北京理工大学");
    </script>
```

## 弹窗 
```javascript
    <script type="text/javascript">
        alert("您点击了地图。");
    </script>
```

# 按键事件

### 单机显示坐标
map.addEventListener('click', function (e) {
    alert('点击位置经纬度：' + e.latlng.lng + ',' + e.latlng.lat);
});

### 只有点击坐标或圆才弹窗
circle.addEventListener('click', overlayStyle);
// 获取覆盖物属性
function overlayStyle(e) {
    var p = e.target;
    if (p instanceof BMapGL.Marker) {
        alert('该覆盖物是点，点的坐标为：' + p.getPosition().lng + ',' + p.getPosition().lat);
    } else if (p instanceof BMapGL.Circle) {
        alert('该覆盖物是圆，其半径为：' + p.getRadius() + '圆中心点坐标：' + p.getCenter().lng + ',' + p.getCenter().lat);
    } else {
        alert('无法获知该覆盖物类型');
    }
}

### 

function showInfo(e){    
    alert(e.point.lng + ", " + e.point.lat);    
    map.removeEventListener("click", showInfo);    
}    
map.addEventListener("click", showInfo);


