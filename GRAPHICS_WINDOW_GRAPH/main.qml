import QtQuick
import QtGraphs

Rectangle{

    color: "#1f211c"
    //Rectangle {id: rect1; color: "green"; width: 500; height: 500 }
    //Rectangle {id: rect2; anchors.left : rect1.right; color: "blue"; width: 500; height: 500 }

    GraphsView 
    {
    id: graph1
    objectName: "GraphObject1"
    //anchors.fill: parent
    //anchors.margins: 20
    width: 500
    height: 500

    theme: GraphsTheme {
        readonly property color c1: "#DBEB00"
        readonly property color c2: "#727548"
        readonly property color c3: Qt.lighter(c2, 1.5)
        colorScheme: GraphsTheme.ColorScheme.Dark
        seriesColors: ["#d5b420", "#eb0000"]
        grid.mainColor: c3
        grid.subColor: c2
        gridVisible: false
        axisX.mainColor: c3
        axisY.mainColor: c3
        axisX.subColor: c2
        axisY.subColor: c2
        axisX.labelTextColor: c1
        axisY.labelTextColor: c1
    }
    axisX: ValueAxis { max: 12; min: 0 }
    axisY: ValueAxis { max: 300; min: 0 }

     seriesList: [ GraphPointsStorage.SeriesPoints,
                   GraphPointsStorage2.SeriesPoints ]

    }
}


