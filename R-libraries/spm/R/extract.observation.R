#' Utility extract function
#'
#' @author Alistair Dunn
#'
"extract.observation" <-
function(lines){
  if(missing(lines)) stop("ERROR: Missing argument lines")
  index.start<-(1:length(lines))[substring(lines,1,1)=="["][1]
  index.end<-(1:length(lines))[substring(lines,1,4)=="*end"][1]
  if(index.start >= index.end) stop("Error")
  res<-list()
  res$report.type<-substring(lines[index.start],14)
  res$label<-substring(lines[index.start+1],2,nchar(lines[index.start+2])-1)
  res$type<-substring(lines[index.start+2],7)
  if(res$type=="abundance") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$catchability<-substring(lines[index.start+5],15)
	this.line<-index.start+5
  else if(res$type=="biomass") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$catchability<-substring(lines[index.start+5],15)
	this.line<-index.start+5
  else if(res$type=="presence") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$catchability<-substring(lines[index.start+5],15)
	this.line<-index.start+5
  else if(res$type=="proportions_at_age") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$min_age<-as.numeric(substring(lines[index.start+5],10))
    res$max_age<-substring(lines[index.start+6],10)
    res$age_plus_group<-substring(lines[index.start+7],17)
	this.line<-index.start+7
  else if(res$type=="proportions_by_category") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$min_age<-as.numeric(substring(lines[index.start+5],10))
    res$max_age<-substring(lines[index.start+6],10)
    res$age_plus_group<-substring(lines[index.start+7],17)
	this.line<-index.start+7
  else if(res$type=="proportions_at_length") {
    res$year<-as.numeric(substring(lines[index.start+3],7))
    res$time_step<-substring(lines[index.start+4],12)
    res$number_of_bins<-as.numeric(substring(lines[index.start+5],17))
    res$length_bins<-substring(lines[index.start+6],14)
	this.line<-index.start+6
  } else {
    stop(paste("Error: Obsevration type '",res$type,"' not known",sep="")) 
  }
  variables<-spm.string.to.vector.of.words(lines[this.line+1])
  data<-spm.string.to.vector.of.words(lines[(this.line+2):(index.end-1)])
  data<-matrix(data,ncol=length(variables),byrow=TRUE)
  data<-as.data.frame(data,stringsAsFactors=FALSE)
  names(data)<-variables
  if("category" %in% variables) {
    data<-data.frame("area"=data[,1], category=data[,2], age=data[,3], observed=data[,4], expected=data[,5],
          residual=data[,6], errorvalue=data[,7], processerror=data[,8], totalerror=data[,9], score=data[,10])
    data$age<-as.numeric(as.character(data$age))
  } else if("age" %in% variables) {
    data<-data.frame("area"=data[,1], age=data[,2], observed=data[,3], expected=data[,4], residual=data[,5],
          errorvalue=data[,6], processerror=data[,7], totalerror=data[,8], score=data[,9])
    data$age<-as.numeric(as.character(data$age))
  } else if("length_bins" %in% variables) {
    data<-data.frame("area"=data[,1], "length_bins"=data[,2], observed=data[,3], expected=data[,4], residual=data[,5],
          errorvalue=data[,6], processerror=data[,7], totalerror=data[,8], score=data[,9])
    data$age<-as.numeric(as.character(data$age))
  } else {
    data<-data.frame("area"=data[,1], observed=data[,2], expected=data[,3], residual=data[,4],
          errorvalue=data[,5], processerror=data[,6], totalerror=data[,7], score=data[,8])
  }
  data$area<-as.character(data$area)
  data$observed<-as.numeric(as.character(data$observed))
  data$expected<-as.numeric(as.character(data$expected))
  data$residual<-as.numeric(as.character(data$residual))
  data$errorvalue<-as.numeric(as.character( data$errorvalue))
  data$processerror<-as.numeric(as.character( data$processerror))
  data$totalerror<-as.numeric(as.character( data$totalerror))
  data$score<-as.numeric(as.character(data$score))
  res$data<-data
  return(res)
}

