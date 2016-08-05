val jsonArrayUDF = udf {
  s:String =>
  if(s.length > 4){
    val trimed = s.substring(1, s.length -1)
    trimed.split(", ").withFilter(_.length > 4).map(_.replace("\"","").trim()).toList
  }else Nil
}
